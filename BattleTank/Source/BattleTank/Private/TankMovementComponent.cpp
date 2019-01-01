#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LTrackToSet, UTankTrack* RTrackToSet)
{
    LTrack = LTrackToSet;
    RTrack = RTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    if (!ensure(LTrack && RTrack)) {return;}
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
    if (!ensure(LTrack && RTrack)) {return;}
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    auto AIForwardIntention = MoveVelocity.GetSafeNormal();
    
    auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention);
    IntendMoveForward(ForwardThrow);
    IntendTurnRight(RightThrow.Z);
}

