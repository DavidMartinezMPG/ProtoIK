// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class PROTOIK_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInteractableComponent* InteractableComponent;

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
};
