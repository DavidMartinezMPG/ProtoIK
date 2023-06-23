// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIKComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> SocketsName;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> SocketsWorldLocation;

protected:
	virtual void BeginPlay() override;

	void TraceFeet();

	UPROPERTY()
	TArray<const USkeletalMeshSocket*> Sockets;

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float GroundTraceDistance;

	UPROPERTY(EditDefaultsOnly)
	float MovementThresholdToTrace;

	FVector LastTraceLocation;
	FVector LastCurrentLocation;
	TArray<FVector> SocketsOffset;
};
