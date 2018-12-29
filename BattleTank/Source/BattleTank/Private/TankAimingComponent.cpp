#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"

UTankAimingComponent::UTankAimingComponent()
{
    bWantsBeginPlay = true;
    PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
    if (!BarrelToSet) {return;}
    Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
    if (!TurretToSet) {return;}
    Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
    if(!Barrel) {return;}
    
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
    
    auto Time = GetWorld()->GetTimeSeconds();
    
    if (bHaveAimSolution)
    {
        auto AimDirection = OutLaunchVelocity.GetSafeNormal();
        MoveBarrelTowards(AimDirection);
        MoveTurretTowards(AimDirection);
        UE_LOG(LogTemp, Warning, TEXT("%f: Aim Solution Found"), Time)
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%f: NO Aim Solution Found"), Time)
    }
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    
    Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
    auto TurretRotator = Turret->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - TurretRotator;
    
    Turret->Rotate(DeltaRotator.Yaw);
}
