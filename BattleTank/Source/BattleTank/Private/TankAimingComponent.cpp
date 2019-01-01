#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"

UTankAimingComponent::UTankAimingComponent()
{
    bWantsBeginPlay = false;
    PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
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
        auto AimDirection = OutLaunchVelocity.GetSafeNormal();
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

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
    if (!ensure(Turret)) {return;}
    auto TurretRotator = Turret->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - TurretRotator;
    
    Turret->Rotate(DeltaRotator.Yaw);
}
