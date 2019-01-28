#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) {return;}
    FoundAimingComponent(AimingComponent);
}
// setting up the possession of tank pawn so pawn can later be depossesed upon death
void ATankPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);
    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!ensure(PossessedTank)) {return;}
        // OnDeath recieved from Tank.cpp through Broadcast(), method is called in ATankPlayer Controller
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
    }
}

void ATankPlayerController::OnPossessedTankDeath()
{
    StartSpectatingOnly(); // depossess pawn
}

void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if (!GetPawn()) {return;}
    // getting a UTankAimingComponent* by casting to the pawn's inherited components that are a UTankAimingComponent
    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) {return;}
    
    FVector HitLocation; // out param
    // true if the method outpouts HitLocation
    bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
    
    if (bGotHitLocation)
    {
        AimingComponent->AimAt(HitLocation); // Calls AimAt in UTankAimingComponent
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const
{
    // get viewport dimensions
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    // calculate the location of desired hit in 3D space (crosshair params are NOT pulled from UI, they are approx)
    auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
    FVector AimDirection; // out parm
    if (GetAimDirection(ScreenLocation, AimDirection))
    {   // out parm is plugged into method to calculate the hit location in 3D space
        // this will return true as long as params are being passed
        return GetLookVectorHitLocation(AimDirection, OutHitLocation);
    }
    return false;
}

bool ATankPlayerController::GetAimDirection(FVector2D ScreenLocation, FVector& AimDirection) const
{
    FVector CameraWorldDirection; // throwing away this parm
    // transform 2D coordinates (ScreenLocation) to a 3D world-space and direction <FVector>(AimDirection)
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldDirection , AimDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector AimDirection, FVector& HitLocation) const
{
    FHitResult HitResult; // using location of hit as out param
    // vectors
    auto StartLocation = PlayerCameraManager->GetCameraLocation();
    auto EndLocation = StartLocation + (AimDirection * LineTraceRange); // range is hardcoded
    // LineTraceSingleByChannel is a bool that traces the world using a specific channel and returns the first blocking hit
    if (GetWorld()->LineTraceSingleByChannel(
                                             HitResult, // out param
                                             StartLocation,
                                             EndLocation,
                                             ECollisionChannel::ECC_Camera // the ray will shoot from the pawn's camera into 3D space
                                             ))
    {
        HitLocation = HitResult.Location; // storing the hit result location to the methods out parm
        return true;
    }
    HitLocation = FVector(0); // defaulted to zero when a world-space hit is not found
    return false;
}
