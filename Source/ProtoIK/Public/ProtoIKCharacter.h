// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProtoIKCharacter.generated.h"

UCLASS()
class PROTOIK_API AProtoIKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProtoIKCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void Interact();

	UPROPERTY(EditAnywhere)
	class UIKMovementComponent* IKMovementComponent;

	UPROPERTY(EditAnywhere)
	class UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere)
	class UIKInteractionComponent* IKInteractionComponent;

	UPROPERTY()
	APlayerCameraManager* CameraManager;
};
