// Fill out your copyright notice in the Description page of Project Settings.


#include "WGExplosiveBarrel.h"

#include "WGMagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AWGExplosiveBarrel::AWGExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->BodyInstance.bNotifyRigidBodyCollision = true;
	
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->Radius = 700;
	RadialForceComponent->ImpulseStrength = 2500;
	// Optional, as the component defaults to affecting ECC_Pawn, ECC_PhysicsBody, ECC_Vehicle, and ECC_Dstructible:
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
}

void AWGExplosiveBarrel::ExplodeOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (AWGMagicProjectile* Projectile = Cast<AWGMagicProjectile>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(TEXT("Boom!")));
		Projectile->Destroy();
		RadialForceComponent->FireImpulse();
	}
}

// Called when the game starts or when spawned
void AWGExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AWGExplosiveBarrel::PostInitializeComponents() {
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AWGExplosiveBarrel::ExplodeOnHit);
}

// Called every frame
void AWGExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

