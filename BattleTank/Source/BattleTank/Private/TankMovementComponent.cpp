#include "TankMovementComponent.h"
#include "BattleTank.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LTrackToSet, UTankTrack* RTrackToSet)
{
    LTrack = LTrackToSet;
    RTrack = RTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    if (!LTrack || !RTrack) {return;}
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(Throw);
}

void UTankMovementComponent::RIntendTurn(float Throw)
{
    if (!LTrack || !RTrack) {return;}
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    auto AIForwardIntention = MoveVelocity.GetSafeNormal();
    auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    IntendMoveForward(ForwardThrow);
}

