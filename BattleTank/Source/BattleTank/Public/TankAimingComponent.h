#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8 {Reloading, Aiming, Locked};

class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    void AimAt(FVector HitLocation, float LaunchSpeed);
    
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EFiringState FiringState = EFiringState::Locked;
    
private:
    UTankAimingComponent();
    
    UTankBarrel* Barrel = nullptr;
    void MoveBarrelTowards(FVector AimDirection);
    
    UTankTurret* Turret = nullptr;
    void MoveTurretTowards(FVector AimDirection);
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 4000;
};
