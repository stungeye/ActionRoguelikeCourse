// Fill out your copyright notice in the Description page of Project Settings.


#include "WGCharacter.h"

// These were forward declared in the header file so
// we need the #includes here.
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

using UEILPS = UEnhancedInputLocalPlayerSubsystem;
using UEIC = UEnhancedInputComponent;

// Sets default values
AWGCharacter::AWGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AWGCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (UEILPS* SubSystem = ULocalPlayer::GetSubsystem<UEILPS>(LocalPlayer)) {
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void AWGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEIC* EnhancedInputComponent = CastChecked<UEIC>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this,
										   &AWGCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this,
										   &AWGCharacter::Rotate);
	}
}

void AWGCharacter::MoveForward(const FInputActionValue& Value) {
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(TEXT("Input: %f"), Value.Get<float>()));
	AddMovementInput(GetActorForwardVector(), Value.Get<float>());
}

void AWGCharacter::Rotate(const FInputActionValue& Value) {
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("Input: %f"), Value.Get<float>()));
	AddControllerYawInput(Value.Get<float>());
}
