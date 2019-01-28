#include "TankMovementComponent.h"
#include "TankTrack.h"

// used in Tank_BP to tie a static mesh to it's cooresponding c++ components
// methods will be called from UTankTrack to move the tank in the world-space
void UTankMovementComponent::Initialise(UTankTrack* LTrackToSet, UTankTrack* RTrackToSet)
{
    LTrack = LTrackToSet;
    RTrack = RTrackToSet;
}

/* player */
// float is recieved from UE specified axis input, which is passed through through Tank_BP
// (gamepad L-thumbstick Y axis)
/* AI */
// float is recieved from RequestDirectMove() which is called via AI pathfinding logic
void UTankMovementComponent::IntendMoveForward(float Throw)
{
    if (!ensure(LTrack && RTrack)) {return;}
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(Throw);
}

/* player */
// float is recieved from UE specified axis input, which is passed through through Tank_BP
// (gamepad L-thumbstick X axis)
/* AI */
// float is recieved from RequestDirectMove() which is called via AI pathfinding logics
void UTankMovementComponent::IntendTurnRight(float Throw)
{
    if (!ensure(LTrack && RTrack)) {return;}
    LTrack->SetThrottle(Throw);
    RTrack->SetThrottle(-Throw);
}

// AI pathfinding logic will call this method automatically due to ATankAIController using MoveToActor()
// this method is provided by the engine
// used to extract MoveVelocity data and use it to apply to the AI movement each frame
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    /* setting up the vectors for product calculations by reducing the vectors' magnitude to 1 with GetSafeNormal() */
    
    // unit vector in the direction the tank is currently facing (by default this is the pawn's X vector in game)
    auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    // unit vector in the direction the AI is intending for the tank to move each frame
    auto AIForwardIntention = MoveVelocity.GetSafeNormal();
    
    /* the angle between TankForward and AIForwardIntention and the AI's intended forward movement speed have a cosine relationship */
    
    // multiply the two vectors and the cosine of the angle between them : this value will be the AI's new intended forward movement speed <float>(-1 to 1)
    auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    // multiply the two vectors and the sine of the angle between them : this value will be the AI's new intended turn movement speed <float>(-1 to 1)
    auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention);
    
    // apply the calculated movement speeds to their cooresponding movement methods
    IntendMoveForward(ForwardThrow);
    // Z vector of RightThrow is the cross product float value
    IntendTurnRight(RightThrow.Z);
}

