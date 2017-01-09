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

		void RecordActor(AActor* actor, FString varName, LogMode mode = LogMode::LogMulti);
		void RecordActors(TArray<AActor*> actors, FString varName, LogMode mode = LogMode::LogMulti);
		void RecordActorOfType(AActor* actor, FString varName, LogMode mode = LogMode::LogMulti);
		void LogAll();

	private:
		struct Recordable
		{
			FString directory;
			FString varName;
			TArray<AActor*> set;
			LogMode mode;

			Recordable() {}
			Recordable(FString d, FString vN, LogMode m)
			{
				directory = d;
				mode = m;
				varName = vN;
			}
		};

		FString _dateTime;
		FString _directory;
		const FString _extension = ".log";
		TArray<Recordable*> recordables;
		unsigned int _frameCount = 0;

		void LogSet(Recordable* recordable); /* CURRENTLY REDUNDANT */
		void LogActor(Recordable* recordable, int index);
		FString CreateDirectory(FString fileName);
		void AppendToFile(FString text, const TCHAR* dir);
		void OverwriteFile(FString text, const TCHAR* dir);
};