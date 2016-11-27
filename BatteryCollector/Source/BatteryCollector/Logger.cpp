// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "Logger.h"

Logger::Logger(FString directory)
{
	_directory = directory;
}

Logger::~Logger()
{
}

void Logger::RecordActor(AActor* actor)
{
	TArray<AActor*> tmp;
	tmp.Add(actor);
	_recordables.Add(tmp);
	_fileDirs.Add(CreateDirectory(actor->GetName()));
}

void Logger::RecordActors(TArray<AActor*> actors)
{
	if (actors.Num() == 0)
		return;

	_recordables.Add(actors);
	_fileDirs.Add(CreateDirectory(actors[0]->GetName()));
}

void Logger::RecordActorOfType(AActor* actor)
{
	UClass* type = actor->GetClass();
	for (int i = 0; i < _recordables.Num(); i++)
	{
		if (type == _recordables[i][0]->GetClass())
		{
			_recordables[i].Add(actor);

			return;
		}
	}

	RecordActor(actor);
}

void Logger::LogAll()
{
	for (int i = 0; i < _recordables.Num(); i++)
	{
		for (AActor* target : _recordables[i])
			AppendToFile("position:" + target->GetActorLocation().ToString() + ";", *_fileDirs[i]);

		AppendToFile(LINE_TERMINATOR, *_fileDirs[i]);
	}
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