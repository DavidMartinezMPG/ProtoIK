// Fill out your copyright notice in the Description page of Project Settings.


#include "IKComponent.h"

#include "Engine/SkeletalMeshSocket.h"

UIKComponent::UIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIKComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SocketsName.IsEmpty()) return;

	MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();

	if (!IsValid(MeshComponent)) return;

	for (const FName SocketName : SocketsName)
	{
		const USkeletalMeshSocket* Socket = MeshComponent->GetSocketByName(SocketName);
		Sockets.Add(Socket);
		SocketsWorldLocation.Add(Socket->GetSocketLocation(MeshComponent));
	}
}

void UIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(MeshComponent)) return;

	for (int32 i = 0; i < Sockets.Num(); ++i)
	{
		const USkeletalMeshSocket* Socket = Sockets[i];
		const FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
		const FVector TraceStartLocation = FVector(SocketLocation.X, SocketLocation.Y, SocketLocation.Z + GroundTraceDistance);
		const FVector TraceEndLocation = FVector(SocketLocation.X, SocketLocation.Y, SocketLocation.Z - GroundTraceDistance);

		DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Red, false, 0.1f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		FHitResult OutHit;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TraceStartLocation, TraceEndLocation, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			SocketsWorldLocation[i].Z = OutHit.Location.Z;

			DrawDebugSphere(GetWorld(), OutHit.Location, 5.f, 5, FColor::Red, false, 0.1f);
		}
	}
}

