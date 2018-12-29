#include "TankAIController.h"
#include "Tank.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
    
    auto PlayerTank = GetPlayerTank();
    if (!PlayerTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI controller can't find player tank"))
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AI found %s"), *(PlayerTank->GetName()));
    }
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (GetPlayerTank())
    {
    // TODO move towards player
    // aim towards player
        GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
    // fire if ready
    }
}
ATank* ATankAIController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
    auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!PlayerTank) {return nullptr;}
    return Cast<ATank>(PlayerTank);
}


