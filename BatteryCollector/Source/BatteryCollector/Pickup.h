// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Returns the mesh for the pickup.
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return pickupMesh; }
	
	// Technically modifiable, but typically shouldn't be modified.
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	// Only run when the execution pin is hit and will save results to output pins.
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool newPickupState);

	/* Expensive calculations that don't change frequently are better as BlueprintCallable, 
	but fast calculations that frequently change are better as BlueprintPure functions. 
	
	See available documentation for more about Blueprint API functions. */

	// Function to call when a pickup is collected.
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

	/* BlueprintNativeEvents split behaviour between code and blueprints but do not require
	code to be overwritten in blueprints to function. */

protected:
	// True when the pickup can be used. false when pickup is de-activated.
	bool isActive;

private:
	// Static mesh to represent pickup in the level.
	// Whilst the mesh is private, we still want to be able to change properties within the level. The UPROPERTY line allows for this.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* pickupMesh;
};
