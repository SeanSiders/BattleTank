#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::SetThrottle(float Throttle)
{
    float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
    DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
    auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
    auto Wheels = GetWheels();
    auto ForcePerWheel = ForceApplied / Wheels.Num();
    for (ASprungWheel* Wheel : Wheels)
    {
        Wheel->AddDrivingForce(ForcePerWheel);
    }
}

TArray<class ASprungWheel*> UTankTrack::GetWheels() const
{
    TArray<ASprungWheel*> Wheels;
    TArray<USceneComponent*> SpawnPoints;
    GetChildrenComponents(true, SpawnPoints);
    for (USceneComponent* Child : SpawnPoints)
    {
        auto SpawnPointChild = Cast<USpawnPoint>(Child);
        if (!SpawnPointChild) continue;
        
        AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
        auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
        if (!SprungWheel) continue;
        
        Wheels.Add(SprungWheel);
    }
    return Wheels;
}
