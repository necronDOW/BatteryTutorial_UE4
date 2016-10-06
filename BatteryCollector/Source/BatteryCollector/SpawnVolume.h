// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Returns the value of whereToSpawn.
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return whereToSpawn; }

	// Find a random point within the BoxComponent.
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

	// Toggle function for the spawning functionality.
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool shouldSpawn);

protected:
	// The pickup to spawn.
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> whatToSpawn;

	FTimerHandle spawnTimer;

	// Minimum spawn delay.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float minSpawnDelay;

	// Maximum spawn delay.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float maxSpawnDelay;

private:
	// Box component to specify where pickups should be spawned.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* whereToSpawn;

	// Handle spawning of a new pickup.
	void SpawnPickup();

	// The current spawn delay.
	float spawnDelay;
};
