// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "TankAIController.generated.h"

class ATank;
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
    
private:
    void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    float AcceptanceRadius = 1000;
};
