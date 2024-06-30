// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WGGamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWGGamePlayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API IWGGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	// Allows for both blueprint and C++ implementations. If we have used
	// BlueprintImplementable then we could only implement the function in a BP.
	UFUNCTION(BlueprintNativeEvent) 
	void Interact(APawn* InstigatorPawn);
};
