// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABatteryPickup();

	// Override pickup behaviour - use Implementation because BlueprintNative.
	void WasCollected_Implementation() override;

	// Accessor function to get the battery power.
	float GetPower();

protected:
	// Set the amount of power the battery gives to the character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected))
	float batteryPower;
};
