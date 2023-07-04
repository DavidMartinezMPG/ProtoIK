// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOIK_API UIKMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIKMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> SocketsName;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> SocketsWorldLocation;

	UPROPERTY(BlueprintReadOnly)
	float BaseSocketHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BaseSocketName;

	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfSocketsAlternating;

protected:
	virtual void BeginPlay() override;

	void TraceSockets(const float DeltaTime);
	void UpdateSocketLocation(const int32 SocketIndex, const FVector CurrentLocation, const FVector NextTraceLocation, const float DeltaTime);
	void UpdateAllSocketsLocation(const FVector CurrentLocation, const FVector NextTraceLocation, const float DeltaTime);

	UPROPERTY()
	TArray<const USkeletalMeshSocket*> Sockets;

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly)
	float MovementThresholdToTrace;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* SocketHeightCurve;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* BaseSocketBobCurve;

	UPROPERTY(EditDefaultsOnly)
	float MaxSocketHeight;

	UPROPERTY(EditDefaultsOnly)
	float MaxBaseSocketBobHeight;

	UPROPERTY()
	const USkeletalMeshSocket* BaseSocket;

	FVector LastTraceLocation;
	TArray<FVector> SocketsOffset;
	int32 CurrentSocketIndex;
	FRotator InitialRotation;
};
