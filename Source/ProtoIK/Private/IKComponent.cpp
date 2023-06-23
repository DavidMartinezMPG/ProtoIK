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

	for (const FName SocketName : SocketsName)
	{
		const USkeletalMeshSocket* Socket = MeshComponent->GetSocketByName(SocketName);
		const FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
		const FVector SocketOffset = SocketLocation - CurrentLocation;
		SocketsOffset.Add(SocketOffset);
		Sockets.Add(Socket);
		SocketsWorldLocation.Add(Socket->GetSocketLocation(MeshComponent));
	}

	LastTraceLocation = CurrentLocation;
	LastCurrentLocation = CurrentLocation;
}

void UIKComponent::TraceFeet()
{
	if (!IsValid(MeshComponent) || !IsValid(MovementComponent)) return;

	FVector Velocity = MovementComponent->Velocity;
	Velocity.Normalize();
	Velocity.Z = 0.f;

	if (Velocity.IsZero()) return;

	const FVector NewTraceLocation = LastTraceLocation + Velocity * MovementThresholdToTrace;
	const FVector CurrentLocation = MeshComponent->GetComponentLocation();

	DrawDebugSphere(GetWorld(), CurrentLocation, 5.f, 5, FColor::Yellow, false, 0.1f);
	DrawDebugSphere(GetWorld(), LastTraceLocation, 5.f, 5, FColor::Blue, false, 0.1f);
	DrawDebugSphere(GetWorld(), NewTraceLocation, 5.f, 5, FColor::Green, false, 0.1f);

	const FVector MovementDelta = CurrentLocation - LastTraceLocation;

	if (MovementDelta.Length() < MovementThresholdToTrace) return;

	for (int32 i = 0; i < Sockets.Num(); ++i)
	{
		const USkeletalMeshSocket* Socket = Sockets[i];
		const FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
		const FVector SocketNewTraceLocation = NewTraceLocation + SocketsOffset[i];
		const FVector SocketToTraceDif = SocketNewTraceLocation - SocketLocation;
		const FVector NewSocketLocation = SocketLocation + SocketToTraceDif;
		const FVector TraceStartLocation = FVector(NewSocketLocation.X, NewSocketLocation.Y, NewSocketLocation.Z + GroundTraceDistance);
		const FVector TraceEndLocation = FVector(NewSocketLocation.X, NewSocketLocation.Y, NewSocketLocation.Z - GroundTraceDistance);

		DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Red, false, 0.1f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		FHitResult OutHit;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TraceStartLocation, TraceEndLocation, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			SocketsWorldLocation[i] = OutHit.Location;

			DrawDebugSphere(GetWorld(), OutHit.Location, 5.f, 5, FColor::Red, false, 0.1f);
		}
	}

	LastTraceLocation = NewTraceLocation;

	// If this frame's delta movement was bigger than one threshold, reset tracing location
	const FVector NewMovementDelta = CurrentLocation - LastTraceLocation;
	if (NewMovementDelta.Length() >= MovementThresholdToTrace)
	{
		LastTraceLocation = CurrentLocation;
	}
}

void UIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceFeet();
}

