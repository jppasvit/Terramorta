// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <vector>
#include "BasicAbility.generated.h"

class LogicTile;
class AVaquerosCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VAQUEROS_API UBasicAbility : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBasicAbility();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ExecuteAbility(LogicTile* source, LogicTile* destination);
	virtual std::vector<LogicTile*> tilesAffected(LogicTile* source);
};
