#include "Tank.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = StartingHealth;
}
// calculates damage to current health, calls tank death event up 0 health which is delegated down to APlayerController
float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
    int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
    int32 DamageToApply = FMath::Clamp<float>(DamageAmount, 0, CurrentHealth);
    
    CurrentHealth -= DamageToApply;
    if (CurrentHealth <= 0)
    {
        OnDeath.Broadcast(); // APlayerController recieves broadcast
    }
    return DamageToApply;
}

float ATank::GetHealthPercent() const
{
    return (float)CurrentHealth / (float)StartingHealth;
}

int32 ATank::SetStartingHealth()
{
    CurrentHealth = StartingHealth;
    return CurrentHealth;
}
