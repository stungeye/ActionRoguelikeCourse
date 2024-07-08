// Fill out your copyright notice in the Description page of Project Settings.


#include "WGInteractionComponent.h"

#include "InputBehavior.h"
#include "WGGamePlayInterface.h"

// Sets default values for this component's properties
UWGInteractionComponent::UWGInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWGInteractionComponent::PrimaryInteract() {
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner{ GetOwner() };

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End{ EyeLocation + (EyeRotation.Vector() * 1000)};

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius{ 30.0f };
	Shape.SetSphere(Radius);
	
	bool bBlockingHit{ GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape) };
	FColor LineColor{ bBlockingHit ? FColor::Green : FColor::Red };

	for(FHitResult Hit : Hits) {
		if (AActor* HitActor{ Hit.GetActor() }) {
			if (HitActor->Implements<UWGGamePlayInterface>()) {
				APawn* OwningPawn = Cast<APawn>(MyOwner);
				IWGGamePlayInterface::Execute_Interact(HitActor, OwningPawn);
				break; // No need to keep looping.
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}


// Called when the game starts
void UWGInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWGInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

