// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BasicAbility.h"
#include "DobleShot.generated.h"

/**
 *
 */
UCLASS()
class VAQUEROS_API UDobleShot : public UBasicAbility
{
	GENERATED_BODY()

public:
	virtual bool ExecuteAbility(LogicTile* source, LogicTile* destination)override;
	virtual std::vector<LogicTile*> tilesAffected(LogicTile* source)override;
};
