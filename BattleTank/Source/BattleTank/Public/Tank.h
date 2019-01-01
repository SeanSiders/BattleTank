#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;
class UTankAimingComponent;

/*
 * player aiming and connecting to ui
*/

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
    void AimAt(FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Firing")
    void Fire();
    
protected:
    UPROPERTY(BlueprintReadOnly)
    UTankAimingComponent* TankAimingComponent = nullptr;
	
private:
    ATank();
    
    void BeginPlay() override;
    
    UTankBarrel* Barrel = nullptr; // TODO remove

    // ld: fire()
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 4000;
    
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint;
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeSeconds = 3;
    
    double LastFireTime = 0;
};
