#include "Tank.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
    int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
    int32 DamageToApply = FMath::Clamp<float>(DamageAmount, 0, CurrentHealth);
    
    CurrentHealth -= DamageToApply;
    return DamageToApply;
}

float ATank::GetHealthPercent() const
{
    return (float)CurrentHealth / (float)StartingHealth;
}
