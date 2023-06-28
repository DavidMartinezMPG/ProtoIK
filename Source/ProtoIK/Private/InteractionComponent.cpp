// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "GameplayTagContainer.h"
#include "InteractableComponent.h"
#include "Components/CapsuleComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interaction Collider"));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::ComponentBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::ComponentEndOverlap);
}

bool UInteractionComponent::TryInteract()
{
	if (!TargetInteractable || !TargetInteractable->CanInteract(this)) return false;

	TargetInteractable->Interact(this);

	return true;
}

bool UInteractionComponent::HasTags(const FGameplayTagContainer Tags) const
{
	return OwnedTags.HasAll(Tags);
}

void UInteractionComponent::GrantTags(const FGameplayTagContainer Tags)
{
	OwnedTags.AppendTags(Tags);
}

void UInteractionComponent::RemoveTags(const FGameplayTagContainer Tags)
{
	OwnedTags.RemoveTags(Tags);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
}

void UInteractionComponent::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	UInteractableComponent* InteractableComponent = OtherActor->GetComponentByClass<UInteractableComponent>();
	if (InteractableComponent)
	{
		InteractablesInRange.Add(InteractableComponent);

		ChooseTargetInteractable();
	}
}

void UInteractionComponent::ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UInteractableComponent* InteractableComponent = OtherActor->GetComponentByClass<UInteractableComponent>();
	if (InteractableComponent)
	{
		InteractablesInRange.Remove(InteractableComponent);

		if (TargetInteractable == InteractableComponent)
		{
			TargetInteractable = nullptr;

			ChooseTargetInteractable();
		}
	}
}

void UInteractionComponent::ChooseTargetInteractable()
{
	if (InteractablesInRange.IsEmpty()) return;

	for (UInteractableComponent* InteractableComponent : InteractablesInRange)
	{
		if (!TargetInteractable)
		{
			TargetInteractable = InteractableComponent;

			continue;
		}

		FVector Location = GetOwner()->GetActorLocation();
		FVector InteractableLocation = InteractableComponent->GetOwner()->GetActorLocation();
		float DistanceToInteractable = FVector::DistSquared(InteractableLocation, Location);
		FVector TargetInteractableLocation = TargetInteractable->GetOwner()->GetActorLocation();
		float DistanceToTargetInteractable = FVector::DistSquared(TargetInteractableLocation, Location);

		if (DistanceToInteractable < DistanceToTargetInteractable)
		{
			TargetInteractable = InteractableComponent;
		}
	}
}
