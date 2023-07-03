// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "InteractionComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInteractableComponent::Interact(UInteractionComponent* InstigatorComponent)
{
	FInteractionTags InteractionTags;

	if (!InstigatorComponent || !CanInteract(InstigatorComponent, InteractionTags)) return false;

	InstigatorComponent->GrantTags(InteractionTags.GrantedTags);
	InstigatorComponent->RemoveTags(InteractionTags.RemovedTags);

	OnInteract.Broadcast(InstigatorComponent, InteractionTags);

	return true;
}

bool UInteractableComponent::CanInteract(const UInteractionComponent* InstigatorComponent, FInteractionTags& OutInteractionTags) const
{
	if (!InstigatorComponent) return false;

	for (const FInteractionTags Tags : InteractionTagsOptions)
	{
		if (InstigatorComponent->HasTags(Tags.RequiredTags))
		{
			OutInteractionTags = Tags;

			return true;
		}
	}

	return false;
}
