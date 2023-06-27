// Fill out your copyright notice in the Description page of Project Settings.


#include "IKComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

UIKComponent::UIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSocketIndex = 0;
	NumberOfSocketsAlternating = 1;
}

void UIKComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SocketsName.IsEmpty()) return;

	MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	MovementComponent = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();

	if (!IsValid(MeshComponent) || !IsValid(MeshComponent)) return;

	const FVector CurrentLocation = MeshComponent->GetComponentLocation();

	BaseSocket = MeshComponent->GetSocketByName(BaseSocketName);

	for (const FName SocketName : SocketsName)
	{
		const USkeletalMeshSocket* Socket = MeshComponent->GetSocketByName(SocketName);
		const FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
		const FVector SocketOffset = SocketLocation - CurrentLocation;
		SocketsOffset.Add(SocketOffset);
		Sockets.Add(Socket);
		SocketsWorldLocation.Add(SocketLocation);
	}

	LastTraceLocation = CurrentLocation;
	NumberOfSocketsAlternating = FMath::Min(NumberOfSocketsAlternating, Sockets.Num());
}

void UIKComponent::TraceSockets(const float DeltaTime)
{
	if (!IsValid(MeshComponent) || !IsValid(MovementComponent)) return;

	FVector Velocity = MovementComponent->Velocity;
	Velocity.Normalize();
	Velocity.Z = 0.f;

	const FVector CurrentLocation = MeshComponent->GetComponentLocation();
	const FVector NextTraceLocation = Velocity.IsZero() ? CurrentLocation : LastTraceLocation + Velocity * MovementThresholdToTrace;

	if (Velocity.IsNearlyZero())
	{
		UpdateAllSocketsLocation(CurrentLocation, NextTraceLocation, DeltaTime);
	}
	else
	{
		const float MovementDelta = (CurrentLocation - LastTraceLocation).Length();
		int32 TempSocketIndex = CurrentSocketIndex;

		for (int32 i = 0; i < NumberOfSocketsAlternating; ++i)
		{
			UpdateSocketLocation(TempSocketIndex, CurrentLocation, NextTraceLocation, DeltaTime);

			TempSocketIndex += 1;
			if (TempSocketIndex >= Sockets.Num())
			{
				TempSocketIndex = 0;
			}
		}

		if (MovementDelta < MovementThresholdToTrace) return;
	}

	CurrentSocketIndex += NumberOfSocketsAlternating;
	if (CurrentSocketIndex >= Sockets.Num())
	{
		CurrentSocketIndex -= Sockets.Num();
	}

	LastTraceLocation = NextTraceLocation;	

	// If this frame's delta movement was bigger than one threshold, reset tracing location
	const float NewMovementDelta = (CurrentLocation - LastTraceLocation).Length();
	if (NewMovementDelta >= MovementThresholdToTrace)
	{
		LastTraceLocation = CurrentLocation;
	}

	DrawDebugSphere(GetWorld(), LastTraceLocation, 5.f, 5, FColor::Blue, false, 0.1f);
}

void UIKComponent::UpdateSocketLocation(const int32 SocketIndex, const FVector CurrentLocation, const FVector NextTraceLocation, const float DeltaTime)
{
	if (!IsValid(MeshComponent) || !IsValid(MovementComponent)) return;

	const FVector CurrentSocketLocation = FMath::Lerp(Sockets[SocketIndex]->GetSocketLocation(MeshComponent), CurrentLocation + SocketsOffset[SocketIndex], DeltaTime * 10.f);

	const FVector SocketTraceStartLocation = FVector(CurrentSocketLocation.X, CurrentSocketLocation.Y, CurrentSocketLocation.Z + TraceDistance);
	const FVector SocketTraceEndLocation = FVector(CurrentSocketLocation.X, CurrentSocketLocation.Y, CurrentSocketLocation.Z - TraceDistance);

	DrawDebugSphere(GetWorld(), CurrentSocketLocation, 5.f, 5, FColor::Green, false, 0.1f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, SocketTraceStartLocation, SocketTraceEndLocation, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		SocketsWorldLocation[SocketIndex] = OutHit.Location;
		SocketsWorldLocation[SocketIndex].Z += 5.f;

		const float MovementDelta = (CurrentLocation - LastTraceLocation).Length();

		if (MovementDelta < MovementThresholdToTrace)
		{
			const float MovementDeltaRemaining = (NextTraceLocation - CurrentLocation).Length();
			const float MovementPercentage = FMath::Clamp((MovementThresholdToTrace - MovementDeltaRemaining) / MovementThresholdToTrace, 0.f, 1.f);

			BaseSocketHeight = BaseSocketBobCurve->GetFloatValue(MovementPercentage) * MaxBaseSocketBobHeight;

			const float Height = SocketHeightCurve->GetFloatValue(MovementPercentage) * MaxSocketHeight;
			SocketsWorldLocation[SocketIndex].Z += Height;
		}

		DrawDebugSphere(GetWorld(), SocketsWorldLocation[SocketIndex], 5.f, 5, FColor::Red, false, 0.1f);
	}
}

void UIKComponent::UpdateAllSocketsLocation(const FVector CurrentLocation, const FVector NextTraceLocation,	const float DeltaTime)
{
	for (int32 i = 0; i < Sockets.Num(); ++i)
	{
		UpdateSocketLocation(i, CurrentLocation, NextTraceLocation, DeltaTime);
	}
}

void UIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceSockets(DeltaTime);
}

