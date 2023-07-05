// Fill out your copyright notice in the Description page of Project Settings.


#include "IKInteractionComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"

UIKInteractionComponent::UIKInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIKInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UpperSocket && MovingUpperSocket)
	{
		const FVector UpperSocketLocation = UpperSocket->GetSocketLocation(MeshComponent);
		const float Distance = FVector::DistSquared(UpperSocketTargetLocation, UpperSocketLocation);
		UE_LOG(LogTemp, Warning, TEXT("%f"), Distance);

		if (Distance <= 200.f)
		{
			MovingUpperSocket = false;

			OnUpperSocketReachedLocation.Broadcast(UpperSocketTargetLocation);
		}
	}

	if (LowerSocket && MovingLowerSocket)
	{
		const FVector LowerSocketLocation = LowerSocket->GetSocketLocation(MeshComponent);
		const float Distance = FVector::DistSquared(LowerSocketTargetLocation, LowerSocketLocation);

		if (Distance <= 200.f)
		{
			MovingLowerSocket = false;

			OnLowerSocketReachedLocation.Broadcast(LowerSocketTargetLocation);
		}
	}
}

void UIKInteractionComponent::MoveUpperSocketTo(const FVector Location)
{
	if (!UpperSocket) return;

	UpperSocketTargetLocation = Location;
	MovingUpperSocket = true;
}

void UIKInteractionComponent::MoveLowerSocketTo(const FVector Location)
{
	if (!LowerSocket) return;

	LowerSocketTargetLocation = Location;
	MovingLowerSocket = true;
}

void UIKInteractionComponent::AttachActorToUpperSocket(AActor* Actor)
{
	if (!UpperSocket && !IsValid(UpperSocketAttachedActor)) return;

	UpperSocketAttachedActor = Actor;
	UpperSocketAttachedActor->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepWorldTransform, UpperSocketName);
}

void UIKInteractionComponent::AttachActorToLowerSocket(AActor* Actor)
{
	if (!LowerSocket && !IsValid(LowerSocketAttachedActor)) return;

	LowerSocketAttachedActor = Actor;
}

void UIKInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!IsValid(Character)) return;

	MeshComponent = Character->GetMesh();

	if (!IsValid(MeshComponent)) return;

	UpperSocket = MeshComponent->GetSocketByName(UpperSocketName);
	LowerSocket = MeshComponent->GetSocketByName(LowerSocketName);
}
