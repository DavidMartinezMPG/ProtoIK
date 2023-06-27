// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "InteractionComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::Interact(UInteractionComponent* InstigatorComponent)
{
	if (!InstigatorComponent) return;

	InstigatorComponent->GrantTags(GrantedTags);
	InstigatorComponent->RemoveTags(RemovedTags);
}

bool UInteractableComponent::CanInteract(const UInteractionComponent* InstigatorComponent) const
{
	if (!InstigatorComponent) return false;

	return InstigatorComponent->HasTags(RequiredTags);
}
