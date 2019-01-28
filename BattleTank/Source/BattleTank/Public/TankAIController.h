#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
    
protected:
    // the distance an AI will stop navigating towards the player
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    float AcceptanceRadius = 8000;
    
private:
    virtual void Tick(float DeltaTime) override;
    
    virtual void SetPawn(APawn* InPawn) override;
    
    UFUNCTION()
    void OnPossessedTankDeath();
};
