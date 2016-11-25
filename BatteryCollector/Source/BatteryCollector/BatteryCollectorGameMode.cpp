// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "BatteryPickup.h"

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

	logDir = FPaths::GameDir() + "Content/Logs/";
	logExt = ".log";
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all spawn volume Actors.
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), foundActors);

	// Add all found actors to an array of spawn volumes.
	for (auto actor : foundActors)
	{
		ASpawnVolume* spawnVolumeActor = Cast<ASpawnVolume>(actor);
		if (spawnVolumeActor)
		{
			spawnVolumeActors.AddUnique(spawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	// Set the score to beat.
	ABatteryCollectorCharacter* myCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (myCharacter)
	{
		powerToWin = myCharacter->GetInitialPower() * 1.25f;
	}

	if (hudWidgetClass != nullptr)
	{
		currentWidget = CreateWidget<UUserWidget>(GetWorld(), hudWidgetClass);
		if (currentWidget != nullptr)
		{
			currentWidget->AddToViewport();
		}
	}
}

void ABatteryCollectorGameMode::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Ensure that the character in question is indeed the battery collector character.
	ABatteryCollectorCharacter* myCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (myCharacter)
	{
		// If the power exceeds the amount needed to win ...
		if (myCharacter->GetCurrentPower() > powerToWin)
		{
			// Set current game state to won.
			SetCurrentState(EBatteryPlayState::EWon);
		}
		// If the character has power, decrease by some fraction of the initial power (1% by default).
		else if (myCharacter->GetCurrentPower() > 0)
		{
			myCharacter->UpdatePower(-deltaTime * decayRate * myCharacter->GetInitialPower());
		}
		else
		{
			// If the player has lost all power, game over.
			SetCurrentState(EBatteryPlayState::EGameOver);
		}

		LogActor(myCharacter);
	}

	TArray<AActor*> batteryPickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABatteryPickup::StaticClass(), batteryPickups);
	LogActors(batteryPickups);
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return powerToWin;
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const
{
	return currentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState newState)
{
	// Set the new state and handle any necessary actions.
	currentState = newState;
	HandleNewState(currentState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState newState)
{
	switch (newState)
	{
		// If the game is playing.
		case EBatteryPlayState::EPlaying:
		{
			// Spawn volumes active.
			for (ASpawnVolume* volume : spawnVolumeActors)
			{
				volume->SetSpawningActive(true);
			}
		}
		break;
		// If we've won the game.
		case EBatteryPlayState::EWon:
		{
			// Spawn volumes in-active.
			for (ASpawnVolume* volume : spawnVolumeActors)
			{
				volume->SetSpawningActive(false);
			}
		}
		break;
		// If we've lost the game.
		case EBatteryPlayState::EGameOver:
		{
			// Spawn volumes in-active.
			for (ASpawnVolume* volume : spawnVolumeActors)
			{
				volume->SetSpawningActive(false);
			}

			// Block player input.
			APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
			if (controller)
			{
				controller->SetCinematicMode(true, false, false, true, true);
			}

			// Ragdoll the character.
			ACharacter* character = UGameplayStatics::GetPlayerCharacter(this, 0);
			if (character)
			{
				character->GetMesh()->SetSimulatePhysics(true);
				character->GetMovementComponent()->MovementState.bCanJump = false;
			}
		}
		break;
		// Unknown/default state.
		default:
		case EBatteryPlayState::EUnknown:
		{
			// Do nothing.
		}
		break;
	}
}

void ABatteryCollectorGameMode::LogActor(AActor* target)
{
	FString fileName = target->GetName() + logExt;
	FString dir = CreateDirectory(logDir, fileName);

	FString output = "name:" + target->GetName() + "\n";
	FFileHelper::SaveStringToFile(output, *dir, FFileHelper::EEncodingOptions::ForceUTF8, 
		&IFileManager::Get(), FILEWRITE_Append);
}

void ABatteryCollectorGameMode::LogActors(TArray<AActor*> targets)
{
	if (targets.Num() == 0)
		return;

	FString fileName = targets[0]->GetName() + "_etc" + logExt;
	FString dir = CreateDirectory(logDir, fileName);

	for (AActor* target : targets)
	{
		FString output = "name:" + target->GetName() + ";";
		FFileHelper::SaveStringToFile(output, *dir, FFileHelper::EEncodingOptions::ForceUTF8,
			&IFileManager::Get(), FILEWRITE_Append);
	}
}

FString ABatteryCollectorGameMode::CreateDirectory(FString fileDir, FString fileName)
{
	IPlatformFile &platformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!platformFile.DirectoryExists(*fileDir))
		platformFile.CreateDirectory(*fileDir);

	if (!platformFile.FileExists(*(fileDir + fileName)))
		FFileHelper::SaveStringToFile("", *(fileDir + fileName));

	return fileDir + fileName;
}