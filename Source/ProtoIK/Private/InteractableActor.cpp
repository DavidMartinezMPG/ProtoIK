// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "InteractableComponent.h"
#include "Components/BoxComponent.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule Component"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->AttachToComponent(BoxComponent, FAttachmentTransformRules::KeepWorldTransform);
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));

	SetRootComponent(BoxComponent);
}
