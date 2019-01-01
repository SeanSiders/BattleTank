#include "Tank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "TankAimingComponent.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
    Super::BeginPlay();
    TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
}

void ATank::AimAt(FVector HitLocation)
{
    if (!ensure(TankAimingComponent)) {return;}
    TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire()
{
    if (!ensure(Barrel)) {return;}
    bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeSeconds;
    
    if (isReloaded)
    {
    auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation("Projectile"), Barrel->GetSocketRotation("Projectile"));
    
    Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
    }
}

