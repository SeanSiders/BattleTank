#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * Drives tank tracks
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Input")
    void IntendMoveForward(float Throw);
    
    UFUNCTION(BlueprintCallable, Category = "Input")
    void IntendTurnRight(float Throw);
    
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialise(UTankTrack* LTrackToSet, UTankTrack* RTrackToSet);
    
private:
    virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
    UTankTrack* LTrack = nullptr;
    UTankTrack* RTrack = nullptr;
};
