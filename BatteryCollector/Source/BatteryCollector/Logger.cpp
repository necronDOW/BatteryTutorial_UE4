// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "Logger.h"
#include "BatteryCollectorCharacter.h"

Logger::Logger(FString directory)
{
	_directory = directory;

	_dateTime = "-" + FDateTime::Now().ToString();
}

Logger::~Logger()
{
}

void Logger::RecordActor(AActor* actor, LogMode mode)
{
	Recordable* tmp = new Recordable(CreateDirectory(actor->GetName() + _dateTime), mode);
	tmp->set.Add(actor);
	recordables.Add(tmp);

	if (mode == LogMode::LogOnce)
		LogSet(tmp);
}

void Logger::RecordActors(TArray<AActor*> actors, LogMode mode)
{
	if (actors.Num() == 0)
		return;

	Recordable* tmp = new Recordable(CreateDirectory(actors[0]->GetName() + _dateTime), mode);
	for (AActor* actor : actors)
		tmp->set.Add(actor);
	recordables.Add(tmp);

	if (mode == LogMode::LogOnce)
		LogSet(tmp);
}

void Logger::RecordActorOfType(AActor* actor, LogMode mode)
{
	UClass* type = actor->GetClass();
	for (int i = 0; i < recordables.Num(); i++)
	{
		Recordable* tmp = recordables[i];
		if (type == tmp->set[0]->GetClass())
		{
			tmp->set.Add(actor);

			if (tmp->mode == LogMode::LogOnce)
				LogSet(tmp);

			return;
		}
	}

	RecordActor(actor, mode);
}

void Logger::LogAll()
{
	for (Recordable* r : recordables)
	{
		if (r->mode == LogMode::LogMulti)
		{
			UClass* type = r->set[0]->GetClass();
			for (AActor* target : r->set)
			{
				AppendToFile("position:" + target->GetActorLocation().ToString() + ";", *r->directory);

				float power = Cast<ABatteryCollectorCharacter>(target)->GetCurrentPower();
				AppendToFile("power:" + FString::SanitizeFloat(power) + ";", *r->directory);
			}

			AppendToFile(LINE_TERMINATOR, *r->directory);
		}
	}
}

void Logger::LogSet(Recordable* recordable)
{
	FString output = "";
	for (AActor* target : recordable->set)
		output += "position:" + target->GetActorLocation().ToString() + ";";

	OverwriteFile(output, *recordable->directory);
}

FString Logger::CreateDirectory(FString fileName)
{
	IPlatformFile &platformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!platformFile.DirectoryExists(*_directory))
		platformFile.CreateDirectory(*_directory);

	FFileHelper::SaveStringToFile("", *(_directory + fileName + _extension));

	return _directory + fileName + _extension;
}

void Logger::AppendToFile(FString text, const TCHAR* dir)
{
	FFileHelper::SaveStringToFile(text, dir, FFileHelper::EEncodingOptions::ForceUTF8,
		&IFileManager::Get(), FILEWRITE_Append);
}

void Logger::OverwriteFile(FString text, const TCHAR* dir)
{
	FFileHelper::SaveStringToFile(text, dir, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get());
}