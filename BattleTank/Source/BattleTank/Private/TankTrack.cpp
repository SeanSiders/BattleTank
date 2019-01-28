#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// calls drive track with the incoming float, clamped
void UTankTrack::SetThrottle(float Throttle)
{
    float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
    DriveTrack(CurrentThrottle);
}

// calculates the force to apply to an individual wheel
void UTankTrack::DriveTrack(float CurrentThrottle)
{
    auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
    auto Wheels = GetWheels();
    auto ForcePerWheel = ForceApplied / Wheels.Num();
    // iterate through an array, recieved from GetWheels(), then apply the  calculated force to each wheel
    // AddDrivingForce is implemented in SprungWheel.cpp
    for (ASprungWheel* Wheel : Wheels)
    {
        Wheel->AddDrivingForce(ForcePerWheel);
    }
}

TArray<class ASprungWheel*> UTankTrack::GetWheels() const
{
    // declaring 2 arrays, Wheels will be filled by iterating through the children of SpawnPoints which happen to be ASprungWheel*
    TArray<ASprungWheel*> Wheels;
    TArray<USceneComponent*> SpawnPoints;
    GetChildrenComponents(true, SpawnPoints); // returns children of SpawnPoints, true ensures ALL decendants of SpawnPoints
    for (USceneComponent* Child : SpawnPoints)
    {
        auto SpawnPointChild = Cast<USpawnPoint>(Child); // to assure we are recieving children of USpawnPoint
        if (!SpawnPointChild) continue;
        
        AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor(); // returns AActor* tied to the child of USpawnPoint
        auto SprungWheel = Cast<ASprungWheel>(SpawnedChild); // to assure we get the AActor* ASprungWheel
        if (!SprungWheel) continue;
        
        Wheels.Add(SprungWheel); // adds the found AActor* to the array "Wheels"
    }
    return Wheels;
}
