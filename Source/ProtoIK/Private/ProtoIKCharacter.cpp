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

