// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocketReachedLocation, FVector, Location);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UIKInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIKInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void MoveUpperSocketTo(const FVector Location);

	UFUNCTION(BlueprintCallable)
	void MoveLowerSocketTo(const FVector Location);

	UFUNCTION(BlueprintCallable)
	void AttachActorToUpperSocket(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void AttachActorToLowerSocket(AActor* Actor);

	UPROPERTY(BlueprintReadOnly)
	FVector UpperSocketTargetLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector LowerSocketTargetLocation;

	UPROPERTY(BlueprintReadOnly)
	AActor* UpperSocketAttachedActor;

	UPROPERTY(BlueprintReadOnly)
	AActor* LowerSocketAttachedActor;

	UPROPERTY(BlueprintAssignable)
	FOnSocketReachedLocation OnUpperSocketReachedLocation;

	UPROPERTY(BlueprintAssignable)
	FOnSocketReachedLocation OnLowerSocketReachedLocation;

	UPROPERTY(BlueprintReadOnly)
	bool MovingUpperSocket;

	UPROPERTY(BlueprintReadOnly)
	bool MovingLowerSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName UpperSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LowerSocketName;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	const USkeletalMeshSocket* UpperSocket;

	UPROPERTY()
	const USkeletalMeshSocket* LowerSocket;

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;
};
