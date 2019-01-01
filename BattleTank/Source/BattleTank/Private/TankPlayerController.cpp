#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) {return;}
    FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if(!ensure(GetControlledTank())) {return;}
    FVector HitLocation; // out param
    if (GetSightRayHitLocation(HitLocation))
    {
        GetControlledTank()->AimAt(HitLocation);
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const
{
    // find crosshair position
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
    // "de-project" screen position
    FVector AimDirection;
    if (GetAimDirection(ScreenLocation, AimDirection))
    {
        GetLookVectorHitLocation(AimDirection, OutHitLocation);
        return true;
    }
    return false;
}

bool ATankPlayerController::GetAimDirection(FVector2D ScreenLocation, FVector& AimDirection) const
{
    FVector CameraWorldDirection;
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldDirection , AimDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector AimDirection, FVector& HitLocation) const
{
    FHitResult HitResult;
    auto StartLocation = PlayerCameraManager->GetCameraLocation();
    auto EndLocation = StartLocation + (AimDirection * LineTraceRange);
    if (GetWorld()->LineTraceSingleByChannel(
                                             HitResult,
                                             StartLocation,
                                             EndLocation,
                                             ECollisionChannel::ECC_Visibility))
    {
        HitLocation = HitResult.Location;
        return true;
    }
    HitLocation = FVector(0);
    return false;
}


