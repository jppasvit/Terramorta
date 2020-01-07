// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BasicAbility.h"
#include "DynamiteAbility.generated.h"
class LogicTile;
/**
 * 
 */
UCLASS()
class VAQUEROS_API UDynamiteAbility : public UBasicAbility
{
	GENERATED_BODY()

	virtual bool ExecuteAbility(LogicTile* source, LogicTile* destination)override;
	virtual std::vector<LogicTile*> tilesAffected(LogicTile* source)override;

public:
	UPROPERTY(EditAnywhere)
	int _damage;
	UPROPERTY(EditAnywhere)
	int _area;
	UPROPERTY(EditAnywhere)
	int _range;
};
