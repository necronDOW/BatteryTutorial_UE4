// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "Pickup.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Sets the initial active state for all pickups to true.
	isActive = true;

	// Create the static mesh component.
	pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = pickupMesh;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Returns active pickup state.
bool APickup::IsActive()
{
	return isActive;
}

// Sets the pickup active state.
void APickup::SetActive(bool newPickupState)
{
	isActive = newPickupState;
}

void APickup::WasCollected_Implementation()
{
	// Log a debug message for what was collected.
	FString pickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected %s"), *pickupDebugString);
}