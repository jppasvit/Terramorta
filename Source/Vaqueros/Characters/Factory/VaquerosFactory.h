// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "VaquerosFactory.generated.h"


UCLASS()
class VAQUEROS_API AVaquerosFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVaquerosFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TMap<FString, UClass*> getBPClasses();
	TMap<FString, UMaterial*> getMaterialClasses();

private:
	TMap<FString, UClass*> _map;
	TMap<FString, UMaterial*> _mapMat;

};
