// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "GameplayTagContainer.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInteractionComponent::HasTags(const TArray<FGameplayTag> Tags) const
{
	for (FGameplayTag Tag : Tags)
	{
		if (!OwnedTags.Contains(Tag)) return false;
	}

	return true;
}

void UInteractionComponent::GrantTags(const TArray<FGameplayTag> Tags)
{
	for (FGameplayTag Tag : Tags)
	{
		OwnedTags.Add(Tag);
	}
}

void UInteractionComponent::RemoveTags(const TArray<FGameplayTag> Tags)
{
	for (FGameplayTag Tag : Tags)
	{
		OwnedTags.Remove(Tag);
	}
}
