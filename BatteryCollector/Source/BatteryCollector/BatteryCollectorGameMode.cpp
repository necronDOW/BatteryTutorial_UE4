// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Base decay rate.
	decayRate = 0.01f;
}

void ABatteryCollectorGameMode::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Ensure that the character in question is indeed the battery collector character.
	ABatteryCollectorCharacter* myCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (myCharacter)
	{
		// If the character has power, decrease by some fraction of the initial power (1% by default).
		if (myCharacter->GetCurrentPower() > 0)
		{
			myCharacter->UpdatePower(-deltaTime * decayRate * myCharacter->GetInitialPower());
		}
	}
}