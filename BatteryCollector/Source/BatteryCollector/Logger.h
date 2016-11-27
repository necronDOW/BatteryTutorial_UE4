// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BatteryCollector.h"

class BATTERYCOLLECTOR_API Logger
{
public:
	Logger(FString directory);
	~Logger();

	void RecordActor(AActor* actor);
	void RecordActors(TArray<AActor*> actors);
	void RecordActorOfType(AActor* actor);
	void LogAll();

private:
	FString _directory;
	const FString _extension = ".log";
	TArray<FString> _fileDirs;
	TArray<TArray<AActor*>> _recordables;

	FString CreateDirectory(FString fileName);
	void AppendToFile(FString text, const TCHAR* dir);
};