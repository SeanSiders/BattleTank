#pragma once

#include "CoreMinimal.h"
#include "TankBarrel.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

 // forward declaration
class UTankBarrel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();
    void AimAt(FVector HitLocation, float LaunchSpeed);
    void SetBarrelReference(UTankBarrel* BarrelToSet);
    
private:
    UTankBarrel* Barrel = nullptr;
    void MoveBarrelTowards(FVector AimDirection);

};
