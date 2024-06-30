// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WGGamePlayInterface.h"
#include "GameFramework/Actor.h"
#include "WGItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AWGItemChest : public AActor, public IWGGamePlayInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float TargetPitch{110};
	
	void Interact_Implementation(APawn* InstigatorPawn) override;
	// Sets default values for this actor's properties
	AWGItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
