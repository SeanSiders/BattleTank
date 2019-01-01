#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/*
 * TankTrack is used to set max drive force to tank movement
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetThrottle(float Throttle);
    
    // Max force per track, in newtons
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    float TrackMaxDrivingForce = 40000000;
};
