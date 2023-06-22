// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoIKCharacter.h"

#include "IKComponent.h"

AProtoIKCharacter::AProtoIKCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	IKComponent = CreateDefaultSubobject<UIKComponent>(TEXT("IK Component"));
}

void AProtoIKCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProtoIKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AProtoIKCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AProtoIKCharacter::MoveRight);
}

void AProtoIKCharacter::MoveForward(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void AProtoIKCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}

