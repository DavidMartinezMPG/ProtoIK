// Fill out your copyright notice in the Description page of Project Settings.


#include "IKComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

UIKComponent::UIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIKComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SocketsName.IsEmpty()) return;

	MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	MovementComponent = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();

	if (!IsValid(MeshComponent) || !IsValid(MeshComponent)) return;

	const FVector CurrentLocation = MeshComponent->GetComponentLocation();

	HipSocket = MeshComponent->GetSocketByName(HipSocketName);

	for (const FName SocketName : SocketsName)
	{
		const USkeletalMeshSocket* Socket = MeshComponent->GetSocketByName(SocketName);
		const FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
		const FVector SocketOffset = SocketLocation - CurrentLocation;
		SocketsOffset.Add(SocketOffset);
		Sockets.Add(Socket);
		SocketsWorldLocation.Add(SocketLocation);
		PreviousSocketsWorldLocation.Add(SocketLocation);
		NextSocketsWorldLocation.Add(SocketLocation);
	}

	LastTraceLocation = CurrentLocation;
	LastCurrentLocation = CurrentLocation;
	CurrentSocketIndex = 0;
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
		UpdateSocketLocation(CurrentSocketIndex, CurrentLocation, NextTraceLocation, DeltaTime);

		const float MovementDelta = (CurrentLocation - LastTraceLocation).Length();

		if (MovementDelta < MovementThresholdToTrace) return;
	}

	CurrentSocketIndex += 1;
	if (CurrentSocketIndex >= Sockets.Num())
	{
		CurrentSocketIndex = 0;
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

void UIKComponent::UpdateSocketLocation(const int32 FootIndex, const FVector CurrentLocation, const FVector NextTraceLocation, const float DeltaTime)
{
	if (!IsValid(MeshComponent) || !IsValid(MovementComponent)) return;

	const FVector CurrentSocketLocation = FMath::Lerp(Sockets[CurrentSocketIndex]->GetSocketLocation(MeshComponent), CurrentLocation + SocketsOffset[CurrentSocketIndex], DeltaTime * 10.f);

	const FVector SocketTraceStartLocation = FVector(CurrentSocketLocation.X, CurrentSocketLocation.Y, CurrentSocketLocation.Z + GroundTraceDistance);
	const FVector SocketTraceEndLocation = FVector(CurrentSocketLocation.X, CurrentSocketLocation.Y, CurrentSocketLocation.Z - GroundTraceDistance);

	DrawDebugSphere(GetWorld(), CurrentSocketLocation, 5.f, 5, FColor::Green, false, 0.1f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, SocketTraceStartLocation, SocketTraceEndLocation, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		SocketsWorldLocation[CurrentSocketIndex] = OutHit.Location;
		SocketsWorldLocation[CurrentSocketIndex].Z += 5.f;

		const float MovementDelta = (CurrentLocation - LastTraceLocation).Length();

		if (MovementDelta < MovementThresholdToTrace)
		{
			const float MovementDeltaRemaining = (NextTraceLocation - CurrentLocation).Length();
			const float MovementPercentage = FMath::Clamp((MovementThresholdToTrace - MovementDeltaRemaining) / MovementThresholdToTrace, 0.f, 1.f);

			HipSocketHeight = HipBobCurve->GetFloatValue(MovementPercentage) * MaxHipBobHeight;

			const float Height = WalkHeightCurve->GetFloatValue(MovementPercentage) * MaxSocketHeight;
			SocketsWorldLocation[CurrentSocketIndex].Z += Height;
		}

		DrawDebugSphere(GetWorld(), SocketsWorldLocation[CurrentSocketIndex], 5.f, 5, FColor::Red, false, 0.1f);
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

