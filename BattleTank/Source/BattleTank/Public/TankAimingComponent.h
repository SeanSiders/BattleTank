#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8 {Reloading, Aiming, Locked};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);
    
    void AimAt(FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Firing")
    void Fire();
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EFiringState FiringState = EFiringState::Locked;
    
private:
    UTankAimingComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
    
    UTankBarrel* Barrel = nullptr;
    void MoveBarrelTowards(FVector AimDirection);
    
    bool IsBarrelMoving();
    
    UTankTurret* Turret = nullptr;
    void MoveTurretTowards(FVector AimDirection);
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 4000;
    
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint;
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeSeconds = 3;
    
    double LastFireTime = 0;
    
    FVector AimDirection;
};
