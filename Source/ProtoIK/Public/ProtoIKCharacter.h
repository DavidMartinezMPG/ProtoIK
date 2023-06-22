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

private:
	UPROPERTY(EditAnywhere)
	class UIKComponent* IKComponent;
};