#include "TankBarrel.h"

// RelativeSpeed is a delta rotation's pitch, which is calculated in UTankAimingComponent
void UTankBarrel::Elevate(float RelativeSpeed)
{
    // assure that the incoming float acts as a percentage
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
    // calculate change in elevation over time (MaxDegreesPerSecond determines the barrel's final movement speed)
    auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    // calculate the resulting pitch elevation
    auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
    // restrict the barrel's elevation to sensible defaults (in degrees)
    auto Elevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
    
    // set the barrel's destination pitch
    SetRelativeRotation(FRotator(Elevation, 0, 0));
}
