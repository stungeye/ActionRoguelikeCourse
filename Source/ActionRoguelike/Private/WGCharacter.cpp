// Fill out your copyright notice in the Description page of Project Settings.


#include "WGCharacter.h"

// These were forward declared in the header file so
// we need the #includes here.
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

using UEILPS = UEnhancedInputLocalPlayerSubsystem;
using UEIC = UEnhancedInputComponent;

// Sets default values
AWGCharacter::AWGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	// Control the spring arm rotation from the controller.
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Don't rotate our character via the controller (spring arm rotation only).
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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
										   &AWGCharacter::MoveRight);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this,
										   &AWGCharacter::Turn);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this,
										   &AWGCharacter::LookUp);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this,
											&AWGCharacter::PrimaryAttack);
	}
}

void AWGCharacter::MoveForward(const FInputActionValue& Value) {
//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(TEXT("Input: %f"), Value.Get<float>()));
	FRotator ControllerRotation{ GetControlRotation() };
	ControllerRotation.Pitch = 0.0f;
	ControllerRotation.Roll = 0.0f;
	AddMovementInput(ControllerRotation.Vector(), Value.Get<float>());
}

void AWGCharacter::MoveRight(const FInputActionValue& Value) {
	FRotator ControllerRotation{ GetControlRotation() };
	ControllerRotation.Pitch = 0.0f;
	ControllerRotation.Roll = 0.0f;

	// Note that X: Forward (Red), Y: Right (Green), Z: Up (Blue)
	// Sneak a peek at the Kismet library's GetRightVector function and find:
	AddMovementInput(FRotationMatrix(ControllerRotation).GetScaledAxis(EAxis::Y), Value.Get<float>());
}

void AWGCharacter::Turn(const FInputActionValue& Value) {
	AddControllerYawInput(Value.Get<float>());
}

void AWGCharacter::LookUp(const FInputActionValue& Value) {
	AddControllerPitchInput(Value.Get<float>());
}

void AWGCharacter::PrimaryAttack() {
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(TEXT("Primary Attack!")));
	FVector HandLocation{ GetMesh()->GetSocketLocation("Muzzle_01") };
	FTransform SpawnTransform{ GetControlRotation(), HandLocation };
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ProjectileClass) {
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
	} else {
		// Log warning here.
	}
}

