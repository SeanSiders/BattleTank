#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
    
    // create a static mesh instance in Projectile_BP and set it as the root component
    CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
    SetRootComponent(CollisionMesh);
    CollisionMesh->SetNotifyRigidBodyCollision(true);
    CollisionMesh->SetVisibility(false);
    
    // create particle system instances in Projectile_BP, which animate the firing and impact of the projectile
    LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
    LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
     
    ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
    ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    ImpactBlast->bAutoActivate = false;
    
    // create a projectile movement instance in Projectile_BP to move projectile along a vector
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
    ProjectileMovement->bAutoActivate = false;
    
    // create a radial force instance in Projectile_BP, used to create the radius where other actors will take damage
    ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
    ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    // upon the projectiles collision mesh hitting another component, call OnHit
    CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// called within UTankAimingComponent::Fire() where LaunchSpeed is passed as the input param
void AProjectile::LaunchProjectile(float Speed)
{
    // apply a velocity to UProjectileMovementComponent, which is exploited from the engine via forward declaration
    ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
    ProjectileMovement->Activate(); // activate movement
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // deactivate the launch animation and begin the explosion animation to simulate projectile impact
    LaunchBlast->Deactivate();
    ImpactBlast->Activate();
    // creates a radial impulse in game, physics/destructible objects will take damage
    ExplosionForce->FireImpulse();
    
    SetRootComponent(ImpactBlast);
    CollisionMesh->DestroyComponent(); // no need for collision mesh, actor instance is no longer needed
    
    // apply damage to actors within the projectile's damage radius
    UGameplayStatics::ApplyRadialDamage(this, ProjectileDamage, GetActorLocation(), ExplosionForce->Radius, UDamageType::StaticClass(), TArray<AActor*>());
    
    // creates a timer w/ a unique handle that will call OnTimerExpire after <float>DestroyDelay
    FTimerHandle Timer;
    GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::OnTimerExpire, DestroyDelay, false);
}

// when called by the timer, the projectile instance is deleted from memory
// this combats the issue of cached projectiles that are no longer in use
void AProjectile::OnTimerExpire()
{
    Destroy();
}
