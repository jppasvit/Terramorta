// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosFactory.h"
#include "Builders/TileMapBuilder.h"
#include "PlatformFilemanager.h"
#include "FileHelper.h"
#include "ConstructorHelpers.h"

// Sets default values
AVaquerosFactory::AVaquerosFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FString loadPath = ATileMapBuilder::getLoadPath();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString dir = FPaths::ProjectContentDir() + "\\SavedScenarios";
	FString path = dir + "\\" + loadPath + ".text";
	FString pathMaterials = dir + "\\" + loadPath + "_materials.text";

	if (loadPath.IsEmpty() || !FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
	{
		//UE_LOG(LogTemp, Error, TEXT("Error: The file doesn't exist."));
	}
	else
	{
		FString blueprints = FString();
		FFileHelper::LoadFileToString(blueprints, *path);

		while(!blueprints.IsEmpty())
		{
			FString temp;
			FString blueprint;
			blueprints.Split("\n", &blueprint, &temp);
			blueprint = blueprint.Replace(TEXT("\r"), TEXT(""));
			FText TestHUDText = FText::FromString(blueprint);
			ConstructorHelpers::FObjectFinder<UClass> myBP(*blueprint);
			_map.Add(blueprint,  myBP.Object);
			//delete myBP;
			blueprints = temp;
		}
		
	}

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*pathMaterials))
	{
		//UE_LOG(LogTemp, Error, TEXT("Error: The materials file doesn't exist."));
	}
	else
	{
		FString materials;
		FFileHelper::LoadFileToString(materials, *pathMaterials);

		while (!materials.IsEmpty())
		{
			FString temp;
			FString material;
			materials.Split("\n", &material, &temp);
			material = material.Replace(TEXT("\r"), TEXT(""));
			FText TestHUDText = FText::FromString(material);
			ConstructorHelpers::FObjectFinder<UMaterial> myMat(*material);
			_mapMat.Add(material, (UMaterial*)myMat.Object);
			materials = temp;
		}

	}

}

// Called when the game starts or when spawned
void AVaquerosFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVaquerosFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TMap<FString, UClass*> AVaquerosFactory::getBPClasses()
{
	return _map;
}

TMap<FString, UMaterial*> AVaquerosFactory::getMaterialClasses()
{
	return _mapMat;
}

