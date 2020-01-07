// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"

/**
 *
 */
class VAQUEROS_API LogicTile
{
public:
	LogicTile();
	~LogicTile();

	LogicTile(AActor *_shadow);
	LogicTile(TSharedPtr<FJsonObject> jsonTile, AActor* occupant, int x, int y, AActor *_shadow);
	LogicTile(bool _walkable, bool _jumpable, int _cost, AActor *_occupant);


	bool _walkable = true;
	bool _jumpable = true;
	int _cost = 0;
	bool _occupied = false;
	AActor *_occupant = nullptr;
	AActor *_shadow = nullptr;
	UMaterial* _material = nullptr;

	bool getOccupied() { return _occupied; }
	void SetOccupant(AActor *object);
	AActor* getOccupant();
	int getCost();
	int getLogicalX() const;
	int getLogicalY() const;
	void SetCoordinates(int x, int y);
	void setWalkable(bool walkable) { _walkable = walkable; }
	void setJumpable(bool jumpable) { _jumpable = jumpable; }
	void setCost(int cost) { _cost = cost; }
	void setMaterial(UMaterial* material);
	TSharedPtr<FJsonObject> toJson();
	FString getOccupantBP();
	FString getMaterialBP();
	void shine(bool mode, int selectDecal);
private:
	int _x;
	int _y;
};