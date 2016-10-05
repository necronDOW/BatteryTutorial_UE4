// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "BatteryCollectorGameMode.generated.h"

// Enumerator to store the current state of gameplay.
UENUM(BlueprintType)
enum class EBatteryPlayState
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float deltaTime) override;

	// Returns the amount of power needed to win - needed for the HUD.
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	// Returns the current play state.
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	// Sets the current playing state.
	void SetCurrentState(EBatteryPlayState newState);

protected:
	// The rate at which the character loses power.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float decayRate;

	// The power needed to win the game.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float powerToWin;

	// Widget class used for the HUD.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> hudWidgetClass;

	// Instance of the widget class.
	UPROPERTY()
	class UUserWidget* currentWidget;

private:
	// Keeps track of the current state of play.
	EBatteryPlayState currentState;
};