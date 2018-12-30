#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TankPlayerController.generated.h"

class ATank;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
    
private:
    void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    ATank* GetControlledTank() const;
    
    void AimTowardsCrosshair();
    
    UPROPERTY(EditDefaultsOnly)
    float CrossHairXLocation = .5;
    UPROPERTY(EditDefaultsOnly)
    float CrossHairYLocation = .33333;
    
    bool GetSightRayHitLocation(FVector &OutHitLocation) const;
    bool GetAimDirection(FVector2D ScreenLocation, FVector& AimDirection) const;
    bool GetLookVectorHitLocation(FVector AimDirection, FVector& HitLocation) const;
    
    float LineTraceRange = 1000000;
};
