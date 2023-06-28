// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoIKCharacter.h"

#include "IKComponent.h"
#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AProtoIKCharacter::AProtoIKCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	IKComponent = CreateDefaultSubobject<UIKComponent>(TEXT("IK Component"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
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
	AddMovementInput(UKismetMathLibrary::GetForwardVector(CameraManager->GetCameraRotation()), Value);
}

void AProtoIKCharacter::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(CameraManager->GetCameraRotation()), Value);
}

void AProtoIKCharacter::Interact()
{
	InteractionComponent->TryInteract();
}
