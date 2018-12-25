// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
    
private:
    ATank* GetControlledTank() const;
    ATank* GetPlayerTank() const;
    void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
};
