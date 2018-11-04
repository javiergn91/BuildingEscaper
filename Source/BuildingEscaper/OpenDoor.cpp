// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include <GameFramework/Actor.h>
#include <GameFramework/PlayerController.h>
#include <Engine/World.h>
#include <Engine/Player.h>
#include <Components/PrimitiveComponent.h>

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (GetPressurePlateTotalMass() > triggerMass) 
	{
		onOpen.Broadcast();
	}
	else
	{
		onClose.Broadcast();
	}
}

float UOpenDoor::GetPressurePlateTotalMass()
{
	TArray<AActor*> actors;
	pressurePlate->GetOverlappingActors(actors);

	float totalMass = 0;

	for (int i = 0; i < actors.Num(); i++) 
	{
		AActor* current = actors[i];
		UPrimitiveComponent* component = current->FindComponentByClass<UPrimitiveComponent>();

		totalMass += component->CalculateMass();
	}


	return totalMass;
}

