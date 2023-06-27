// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	UFUNCTION(BlueprintCallable)
	bool HasTags(const TArray<FGameplayTag> Tags) const;

	UFUNCTION(BlueprintCallable)
	void GrantTags(const TArray<FGameplayTag> Tags);

	UFUNCTION(BlueprintCallable)
	void RemoveTags(const TArray<FGameplayTag> Tags);

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> OwnedTags;
};
