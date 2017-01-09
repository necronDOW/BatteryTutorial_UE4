// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "Logger.h"
#include "BatteryCollectorCharacter.h"

Logger::Logger(FString directory)
{
	_directory = directory;

	_dateTime = FDateTime::Now().ToString();
}

Logger::~Logger()
{
}

void Logger::RecordActor(AActor* actor, FString varName, LogMode mode)
{
	Recordable* tmp = new Recordable(CreateDirectory(actor->GetName() + "~" + _dateTime + "~" + varName), varName, mode);
	tmp->set.Add(actor);
	recordables.Add(tmp);

	if (mode == LogMode::LogOnce)
		LogSet(tmp);
}

void Logger::RecordActors(TArray<AActor*> actors, FString varName, LogMode mode)
{
	if (actors.Num() == 0)
		return;

	Recordable* tmp = new Recordable(CreateDirectory(actors[0]->GetName() + "~" + _dateTime + "~" + varName), varName, mode);
	for (AActor* actor : actors)
		tmp->set.Add(actor);
	recordables.Add(tmp);

	if (mode == LogMode::LogOnce)
		LogSet(tmp);
}

void Logger::RecordActorOfType(AActor* actor, FString varName, LogMode mode)
{
	UClass* type = actor->GetClass();
	for (int i = 0; i < recordables.Num(); i++)
	{
		Recordable* tmp = recordables[i];
		if (type == tmp->set[0]->GetClass())
		{
			tmp->set.Add(actor);

			if (tmp->mode == LogMode::LogOnce)
				LogActor(tmp, tmp->set.Num() - 1);

			return;
		}
	}

	RecordActor(actor, varName, mode);
}

void Logger::LogAll()
{
	_frameCount++;

	for (Recordable* r : recordables)
	{
		if (r->mode == LogMode::LogMulti)
		{
			UClass* type = r->set[0]->GetClass();
			for (AActor* target : r->set)
			{
				FString frameStr = FString::FromInt(_frameCount);

				if (r->varName == "position")
					AppendToFile(target->GetActorLocation().ToString() + " F=" + frameStr + ";", *r->directory);

				if (r->varName == "power")
				{
					float power = Cast<ABatteryCollectorCharacter>(target)->GetCurrentPower();
					AppendToFile(FString::SanitizeFloat(power) + " F=" + frameStr + ";", *r->directory);
				}
			}

			AppendToFile(LINE_TERMINATOR, *r->directory);
		}
	}
}

/* CURRENTLY REDUNDANT */
void Logger::LogSet(Recordable* recordable)
{
	FString output = "";
	for (AActor* target : recordable->set)
		output += target->GetActorLocation().ToString() + " F=" + FString::FromInt(_frameCount) + ";" + LINE_TERMINATOR;

	OverwriteFile(output, *recordable->directory);
}
/* CURRENTLY REDUNDANT */

void Logger::LogActor(Recordable* recordable, int index)
{
	AActor* target = recordable->set[index];
	AppendToFile(target->GetActorLocation().ToString() + " F=" + FString::FromInt(_frameCount) + ";" + LINE_TERMINATOR, *recordable->directory);
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