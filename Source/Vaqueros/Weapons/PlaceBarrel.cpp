// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceBarrel.h"
#include "MatrixLogic/LogicTile.h"
#include "MatrixLogic/Matriz.h"
#include "GameController/VaquerosGameManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Characters/Characters/Wall.h"

UPlaceBarrel::UPlaceBarrel() {
	FString dir = FString("	Class'/Game/Blueprints/AbilityBarrel.AbilityBarrel_C'");
	ConstructorHelpers::FObjectFinder<UClass> aux(*dir);
	barrelClass = aux.Object;

}

bool UPlaceBarrel::ExecuteAbility(LogicTile * source, LogicTile * destination)
{
	if (destination && !destination->getOccupied()) {
		Matriz* matriz = VaquerosGameManager::GetInstance()->GetMatrix();
		barrel = GetWorld()->SpawnActor<AWall>(barrelClass, matriz->locationTile(destination), FRotator(0, 0, 0));
		destination->SetOccupant(barrel);
		return true;
	}
	return false;
}

std::vector<LogicTile*> UPlaceBarrel::tilesAffected(LogicTile * source)
{
	Matriz* matriz = VaquerosGameManager::GetInstance()->GetMatrix();
	std::vector<LogicTile*> tiles;
	int X = source->getLogicalX(), Y = source->getLogicalY();;
	LogicTile *tile = matriz->GetTile(X + 1, Y);
	if (tile && !tile->getOccupied())
		tiles.push_back(tile);

	tile = matriz->GetTile(X - 1, Y);
	if (tile && !tile->getOccupied())
		tiles.push_back(tile);

	tile = matriz->GetTile(X, Y + 1);
	if (tile && !tile->getOccupied())
		tiles.push_back(tile);

	tile = matriz->GetTile(X, Y - 1);
	if (tile && !tile->getOccupied())
		tiles.push_back(tile);
	return tiles;
}
