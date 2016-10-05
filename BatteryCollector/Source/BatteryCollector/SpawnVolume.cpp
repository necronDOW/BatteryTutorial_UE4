// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the box component to represent the spawn volume.
	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = whereToSpawn;

	// Set the spawn delay range.
	minSpawnDelay = 1.0f;
	maxSpawnDelay = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	// Get a random spawn delay between min and max.
	spawnDelay = FMath::FRandRange(minSpawnDelay, maxSpawnDelay);
	GetWorldTimerManager().SetTimer(spawnTimer, this, &ASpawnVolume::SpawnPickup, spawnDelay, false);
}

// Called every frame
void ASpawnVolume::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector spawnOrigin = whereToSpawn->Bounds.Origin;
	FVector spawnExtent = whereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void ASpawnVolume::SpawnPickup()
{
	// Ensure that whatToSpawn is set before spawning.

	if (whatToSpawn != NULL)
	{
		// Ensure that the world is valid.
		UWorld* const world = GetWorld();
		if (world)
		{
			// Set up spawning parameters.
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			// Get a random spawning location.
			FVector spawnLocation = GetRandomPointInVolume();

			// Set a random rotation for the spawned item.
			float randomRotation = FMath::FRand() * 360.0f;
			FRotator spawnRotation;
			spawnRotation.Yaw = randomRotation;
			spawnRotation.Pitch = randomRotation;
			spawnRotation.Roll = randomRotation;

			// Spawn a pickup with the generated spawn parameters.
			APickup* const spawnedPickup = world->SpawnActor<APickup>(whatToSpawn, spawnLocation, spawnRotation, spawnParams);
		
			// Get a random spawn delay between min and max.
			spawnDelay = FMath::FRandRange(minSpawnDelay, maxSpawnDelay);
			GetWorldTimerManager().SetTimer(spawnTimer, this, &ASpawnVolume::SpawnPickup, spawnDelay, false);
		}
	}
}