#include "TankTurret.h"

// RelativeSpeed is a delta rotation's pitch, which is calculated in UTankAimingComponent
void UTankTurret::Rotate(float RelativeSpeed)
{
    // assure that the incoming float acts as a percentage
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
    // calculate change in yaw over time (MaxDegreesPerSecond determines the turret's final movement speed)
    auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    // calculate the resulting yaw rotation
    auto Rotation = RelativeRotation.Yaw + RotationChange;
    // set the turret's yaw
    SetRelativeRotation(FRotator(0, Rotation, 0));
}
