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

	UPROPERTY(BlueprintReadOnly)
	float HipSocketHeight;

protected:
	virtual void BeginPlay() override;

	void TraceSockets(const float DeltaTime);
	void UpdateSocketLocation(const int32 FootIndex, const FVector CurrentLocation, const FVector NextTraceLocation, const float DeltaTime);
	void UpdateAllSocketsLocation(const FVector CurrentLocation, const FVector NextTraceLocation, const float DeltaTime);

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

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* WalkHeightCurve;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* HipBobCurve;

	UPROPERTY(EditDefaultsOnly)
	float MaxSocketHeight;

	UPROPERTY(EditDefaultsOnly)
	float MaxHipBobHeight;

	UPROPERTY(EditDefaultsOnly)
	FName HipSocketName;

	UPROPERTY()
	const USkeletalMeshSocket* HipSocket;

	TArray<FVector> PreviousSocketsWorldLocation;
	TArray<FVector> NextSocketsWorldLocation;
	FVector LastTraceLocation;
	FVector LastCurrentLocation;
	TArray<FVector> SocketsOffset;
	int32 CurrentSocketIndex;
};
