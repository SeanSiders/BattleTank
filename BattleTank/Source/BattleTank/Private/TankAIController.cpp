#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);
    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!PossessedTank) {return;}
        // OnDeath recieved from Tank.cpp through Broadcast(), method is called in ATankAIController
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
    }
}
// AI tank death has same effect as ATankPlayerController's, using a different method to depossess
void ATankAIController::OnPossessedTankDeath()
{
    if (!GetPawn()) {return;}
    GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // getting pointers - AActor*
    auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
    auto ControlledTank = GetPawn();
    
    if (!(PlayerTank && ControlledTank)) {return;}
    
    // calling AI to navigate within the 3D nav mesh specified within UE editor
    // this will also call a RequestDirectMove() in the background, which is further tweeked in UTankMovementComponent
    // AcceptanceRadius will provide a stopping distance for AI from player
    MoveToActor(PlayerTank, AcceptanceRadius);
    
    // UTankAimingComponent*
    auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
    // calling AI to aim at player's 3D vector location
    AimingComponent->AimAt(PlayerTank->GetActorLocation());
    // checking the firing state, which is a 4 param struct to simulate reload time
    if (AimingComponent->GetFiringState() == EFiringState::Locked)
    {
        AimingComponent->Fire();
    }
}


