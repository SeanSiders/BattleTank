#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:
    
    void LaunchProjectile(float Speed);
    
private:
    UProjectileMovementComponent* ProjectileMovement = nullptr;
    
    /* collision detection for the projectile */
    /* passing in the primitive component projectile and the actor that collides with it, a hit result is output */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    void OnTimerExpire();
    
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    float DestroyDelay = 10;
    
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    float ProjectileDamage = 20;
    
    /* components appear in Projectile_BP with params that are defined in AProjectile::AProjectile() */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* CollisionMesh = nullptr;
    
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UParticleSystemComponent* LaunchBlast = nullptr;
    
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UParticleSystemComponent* ImpactBlast = nullptr;
    
    UPROPERTY(VisibleAnywhere, Category = "Components")
    URadialForceComponent* ExplosionForce = nullptr;
};
