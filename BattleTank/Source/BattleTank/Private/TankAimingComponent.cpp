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
    LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeSeconds)
    {
        FiringState = EFiringState::Reloading;
    }
    else if (IsBarrelMoving())
    {
        FiringState = EFiringState::Aiming;
    }
    else
    {
        FiringState = EFiringState::Locked;
    }
}

EFiringState UTankAimingComponent::GetFiringState() const
{
    return FiringState;
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
    if(!ensure(Barrel)) {return;}
    
    FVector OutLaunchVelocity;
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
    FVector EndLocation = HitLocation;
    
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
        (
         this,
         OutLaunchVelocity,
         StartLocation,
         EndLocation,
         LaunchSpeed,
         false,
         0, 0,
         ESuggestProjVelocityTraceOption::DoNotTrace
        );
    
    if (bHaveAimSolution)
    {
        AimDirection = OutLaunchVelocity.GetSafeNormal();
        MoveBarrelTowards(AimDirection);
        MoveTurretTowards(AimDirection);
    }
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
    if (!ensure(Barrel)) {return;}
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    
    Barrel->Elevate(DeltaRotator.Pitch);
}

bool UTankAimingComponent::IsBarrelMoving()
{
    if (!ensure(Barrel)) {return false;}
    auto BarrelForward = Barrel->GetForwardVector();
    return !BarrelForward.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
    if (!ensure(Turret)) {return;}
    auto TurretRotator = Turret->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - TurretRotator;
    
    if (DeltaRotator.Yaw < 180)
    {
        Turret->Rotate(DeltaRotator.Yaw);
    }
    else
    {
        Turret->Rotate(-DeltaRotator.Yaw);
    }
}

void UTankAimingComponent::Fire()
{
    if (FiringState != EFiringState::Reloading)
    {
        if (!ensure(Barrel)) {return;}
        if (!ensure(ProjectileBlueprint)) {return;}
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation("Projectile"), Barrel->GetSocketRotation("Projectile"));
        
        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
    }
}
