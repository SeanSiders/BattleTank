#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

UTankAimingComponent::UTankAimingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
    Super::BeginPlay();
    // initialise last time fired, by default, tanks cannot fire until <float>ReloadTimeSeconds after Super::BeginPlay()
    LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{   // determines the current firing state to simulate ammo reloading
    if (RoundsLeft <= 0)
    {
        FiringState = EFiringState::OutOfAmmo;
    }
    else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeSeconds)
    {
        FiringState = EFiringState::Reloading;
    }
    else if (IsBarrelMoving()) // checks to see if barrel is moving to UI/AI intended aim direction
    {
        FiringState = EFiringState::Aiming;
    }
    else // barrel is no longer moving
    {
        FiringState = EFiringState::Locked;
    }
}

// used exlusively in PlayerUI_BP to display on screen ammunition supply (converting int32 to text)
int32 UTankAimingComponent::GetRoundsLeft() const
{
    return RoundsLeft;
}

// used exlusively by ATankAIController to determine whether AI can/should fire
EFiringState UTankAimingComponent::GetFiringState() const
{
    return FiringState;
}

// takes in UStaticMeshComponents from Tank_BP and assigns them to c++ components
void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

// HitLocation passed down from ATankAIController : it is the player's 3D vector location
void UTankAimingComponent::AimAt(FVector HitLocation)
{
    if(!ensure(Barrel)) {return;}
    
    FVector OutLaunchVelocity; // out parm
    // tying a vector to a socket specified in the Barrel static mesh
    // socket is reference by FName string
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
    FVector EndLocation = HitLocation; // assigning player location to a 2nd vector
    
    // calculates a vector that resembles the projectile's velocity (distance/time) + 3D direction
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
        (
         this, // using this component
         OutLaunchVelocity, // out parm
         StartLocation,
         EndLocation,
         LaunchSpeed,
         false, // of the 2 possible arcs given a velocity (projectile physics), take the low arc
         0, 0, // radius of collision = 0 override gravity = 0
         ESuggestProjVelocityTraceOption::DoNotTrace
        );
    
    if (bHaveAimSolution)
    {
        // AimDirection declared in UTankAimingComponent.h
        // GetSafeNormal() reduces vector values to (minimun vector length) ^ 2
        AimDirection = OutLaunchVelocity.GetSafeNormal();
        // Calling methods using the AimDirection vector
        MoveBarrelTowards(AimDirection);
        MoveTurretTowards(AimDirection);
    }
}

void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{
    if (!ensure(Barrel)) {return;}
    // get the barrel's current forward vector rotation, this will only be a variance in pitch
    // pitch value is clamped in UTankBarrel.cpp
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    // get the calculated aim direction rotation
    auto AimAsRotator = TargetAimDirection.Rotation();
    // calculates the difference of intended aim direction rotation and the barrel's current rotation
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    
    // apply the calculated delta pitch to the barrel
    Barrel->Elevate(DeltaRotator.Pitch);
}

// if the barrel's forward vector is within .01 of the intended aim direction, return true
bool UTankAimingComponent::IsBarrelMoving()
{
    if (!ensure(Barrel)) {return false;}
    auto BarrelForward = Barrel->GetForwardVector();
    return !BarrelForward.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
    if (!ensure(Turret)) {return;}
    // get the turret's forward vector rotation, this will only be a variance in yaw
    // yaw value is clamped in UTankTurret.cpp
    auto TurretRotator = Turret->GetForwardVector().Rotation();
    // get the calculated aim direction rotation
    auto AimAsRotator = AimDirection.Rotation();
    // calculate the difference of intended aim direction rotation and the turret's current rotation
    auto DeltaRotator = AimAsRotator - TurretRotator;
    
    // if DeltaRotator's absolute value of yaw is less than 180, rotate in counter-clockwise (positive)
    // ensures turret always takes the shortest rotation path to the intended aim direction rotation
    if (FMath::Abs(DeltaRotator.Yaw) < 180)
    {
        Turret->Rotate(DeltaRotator.Yaw);
    }
    else
    {
        Turret->Rotate(-DeltaRotator.Yaw);
    }
}

void UTankAimingComponent::Fire() // called from ATankAIController
{
    if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
    {
        if (!ensure(Barrel)) {return;}
        if (!ProjectileBlueprint) {return;}
        // AProjectile* spawns in the world, using information in Projectile_BP
        // spawn location/rotation is from a socket in the barrel's static mesh
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation("Projectile"), Barrel->GetSocketRotation("Projectile"));
        
        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds(); // assign double val, from timer
        RoundsLeft--;
    }
}


