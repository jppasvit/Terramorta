// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JsonObject.h"
#include "VaquerosActor.generated.h"

UCLASS()
class VAQUEROS_API AVaquerosActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVaquerosActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void fromJson();
	virtual TSharedPtr<FJsonObject> toJson();

};
