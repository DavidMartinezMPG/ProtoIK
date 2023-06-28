// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UInteractableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	bool TryInteract();

	UFUNCTION(BlueprintCallable)
	bool HasTags(const FGameplayTagContainer Tags) const;

	UFUNCTION(BlueprintCallable)
	void GrantTags(const FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable)
	void RemoveTags(const FGameplayTagContainer Tags);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ChooseTargetInteractable();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer OwnedTags;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY()
	TArray<UInteractableComponent*> InteractablesInRange;

	UPROPERTY()
	UInteractableComponent* TargetInteractable;
};
