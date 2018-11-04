// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	input = GetOwner()->FindComponentByClass<UInputComponent>();

	if (input)
	{
		input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent missing in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
	FHitResult hit = GetFirstPhysicsBodyInReach();

	if (hit.GetActor())
	{
		UPrimitiveComponent* component = hit.GetComponent();
		physicsHandle->GrabComponent(component, "", component->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	physicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector location;
	FRotator rotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(location, rotation);
	FVector destination = location + rotation.Vector() * reach;

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		hit,
		location,
		destination,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(), false, GetOwner())
	);

	return hit;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector location;
	FRotator rotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(location, rotation);
	FVector destination = location + rotation.Vector() * reach;

	return destination;
}
