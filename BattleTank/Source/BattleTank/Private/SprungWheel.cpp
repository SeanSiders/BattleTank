#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    
    // this constraint will act as suspension for the vehicle
    MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
    SetRootComponent(MassWheelConstraint);
    
    Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
    Axle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    
    Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
    Wheel->SetupAttachment(Axle);
    
    // this constraint will act as the wheel barring
    AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
    AxleWheelConstraint->SetupAttachment(Axle);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
    
    // when the wheel is grounded, call OnHit
    Wheel->SetNotifyRigidBodyCollision(true);
    Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
    
    SetupConstraints();
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // TG_PostPhysics is specifying a tick that implements after physics are apply that frame
    if (GetWorld()->TickGroup == TG_PostPhysics)
    {
        // this resets the vehicle's movement force so magnitude does not build exponentially
        TotalForceMagnitudeThisFrame = 0;
    }
}

void ASprungWheel::SetupConstraints()
{
    if (!GetAttachParentActor()) {return;}
    // getting the root component of the owning tank, this will serve as on of the vehicle shock restraints
    UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
    if (!BodyRoot) {return;}
    // setting the "shock" constraint between vehicle and axle
    MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
    // setting the "barring constraint between axle and wheel
    AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

// called from UTankTrack, where the force is calculated from user input
// user input X axis and Y axis values are summed in ForceMagnitude
void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
    TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // when wheel is grounded
    ApplyForce();
}

void ASprungWheel::ApplyForce()
{
    // add force to the axle's forward vector in proportion to the current force magnitude
    Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

