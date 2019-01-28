#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

/*
 * Basic initilization of tank "health". All other characteristics are iniherited by components.
*/

// creating FTankDelegate to trigger an event using Broacast()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
    
public:
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
    
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent() const;
    
    UFUNCTION(BlueprintPure, Category = "Health")
    int32 SetStartingHealth();
    
    FTankDelegate OnDeath;
    
private:
    ATank();
    
    virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    int32 StartingHealth = 100;
    
    UPROPERTY(VisibleAnywhere, Category = "Setup")
    int32 CurrentHealth;
};
