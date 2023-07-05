// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoIKCharacter.h"

#include "IKInteractionComponent.h"
#include "IKMovementComponent.h"
#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AProtoIKCharacter::AProtoIKCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	IKMovementComponent = CreateDefaultSubobject<UIKMovementComponent>(TEXT("IK Movement Component"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	IKInteractionComponent = CreateDefaultSubobject<UIKInteractionComponent>(TEXT("IK Interaction Component"));
}

void AProtoIKCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void AProtoIKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AProtoIKCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AProtoIKCharacter::MoveRight);

	PlayerInputComponent->BindAction(FName("Interact"), EInputEvent::IE_Pressed, this, &AProtoIKCharacter::Interact);
}

void AProtoIKCharacter::MoveForward(float Value)
{
	if (IKInteractionComponent->MovingUpperSocket || IKInteractionComponent->MovingLowerSocket) return;

	AddMovementInput(UKismetMathLibrary::GetForwardVector(CameraManager->GetCameraRotation()), Value);
}

void AProtoIKCharacter::MoveRight(float Value)
{
	if (IKInteractionComponent->MovingUpperSocket || IKInteractionComponent->MovingLowerSocket) return;

	AddMovementInput(UKismetMathLibrary::GetRightVector(CameraManager->GetCameraRotation()), Value);
}

void AProtoIKCharacter::Interact()
{
	InteractionComponent->TryInteract();
}
