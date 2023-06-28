// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class UInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, UInteractionComponent*, InstigatorComponent);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer RemovedTags;

	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;

private:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer RequiredTags;
};
