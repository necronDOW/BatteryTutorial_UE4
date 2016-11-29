// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BatteryCollector.h"

class BATTERYCOLLECTOR_API Logger
{
	public:
		enum LogMode
		{
			LogOnce,
			LogMulti
		};

		Logger(FString directory);
		~Logger();

		void RecordActor(AActor* actor, LogMode mode = LogMode::LogMulti);
		void RecordActors(TArray<AActor*> actors, LogMode mode = LogMode::LogMulti);
		void RecordActorOfType(AActor* actor, LogMode mode = LogMode::LogMulti);
		void LogAll();

	private:

		FString _directory;
		const FString _extension = ".log";
		TArray<FString> _fileDirs;
		TArray<TArray<AActor*>> _recordables;
		TArray<LogMode> _logModes;

		void LogSet(int index);
		FString CreateDirectory(FString fileName);
		void AppendToFile(FString text, const TCHAR* dir);
		void OverwriteFile(FString text, const TCHAR* dir);
};