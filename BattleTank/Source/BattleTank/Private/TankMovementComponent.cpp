#include "TankMovementComponent.h"
#include "BattleTank.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LTrackToSet, UTankTrack* RTrackToSet)
{
    if (!LTrackToSet || !RTrackToSet) {return;}
    LTrack = LTrackToSet;
    RTrack = RTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    if (Throw != 0)
    {
    UE_LOG(LogTemp, Warning, TEXT("LAnalog Y Axis Input: %f"), Throw)
    }
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(Throw);
}


