// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class UInteractionComponent;

USTRUCT(BlueprintType)
struct PROTOIK_API FInteractionTags
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer RemovedTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer RequiredTags;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteract, UInteractionComponent*, InstigatorComponent, FInteractionTags, InteractionTags);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UFUNCTION(BlueprintCallable)
	bool Interact(UInteractionComponent* InstigatorComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInteractionTags> InteractionTagsOptions;

	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;

private:
	bool CanInteract(const UInteractionComponent* InstigatorComponent, FInteractionTags& OutInteractionTags) const;
};
