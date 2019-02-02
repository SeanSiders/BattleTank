#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8 {Reloading, Aiming, Locked, OutOfAmmo};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    // Tank_BP function that ties a static mesh to it's cooresponding c++ components
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);
    
    void AimAt(FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Firing")
    void Fire();
    
    EFiringState GetFiringState() const;
    
    UFUNCTION(BlueprintCallable, Category = "Firing")
    int32 GetRoundsLeft() const;
    
protected:
    // used to assign UI crosshair color in relation to EFiringState in PlayerUI_BP
    // PlayerUI_BP has a reference to UTankAimingComponent through a blueprint pointer variable
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EFiringState FiringState = EFiringState::Reloading;
    
private:
    UTankAimingComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
    
    UTankBarrel* Barrel = nullptr; // assigned in Initialize()
    void MoveBarrelTowards(FVector AimDirection);
    
    bool IsBarrelMoving();
    
    UTankTurret* Turret = nullptr; // assigned in Initialize()
    void MoveTurretTowards(FVector AimDirection);
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 8000;
    
    // reference to Projectile_BP
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint;
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeSeconds = 2;
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    int32 RoundsLeft = 20;
    
    double LastFireTime = 0;
    
    FVector AimDirection;
};
