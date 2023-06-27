// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class UInteractionComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UFUNCTION(BlueprintCallable)
	void Interact(UInteractionComponent* InstigatorComponent);

	UFUNCTION(BlueprintCallable)
	bool CanInteract(const UInteractionComponent* InstigatorComponent) const;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> GrantedTags;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> RemovedTags;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> RequiredTags;
};
