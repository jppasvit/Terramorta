// Fill out your copyright notice in the Description page of Project Settings.

#include "DobleShot.h"
#include "Characters/Characters/VaquerosCharacter.h"
#include "MatrixLogic/LogicTile.h"
#include "MatrixLogic/Matriz.h"

bool UDobleShot::ExecuteAbility(LogicTile* source, LogicTile* destination) {
	AVaquerosCharacter* owner = Cast<AVaquerosCharacter>(GetOwner());
	if (owner) {
		Matriz* matriz = VaquerosGameManager::GetInstance()->GetMatrix();
		owner->makeShot(matriz->locationTile(destination));
		owner->makeShot(matriz->locationTile(destination));
		return true;
	}
	return false;
}

std::vector<LogicTile*> UDobleShot::tilesAffected(LogicTile * source)
{
	AVaquerosCharacter* owner = Cast<AVaquerosCharacter>(GetOwner());
	if (owner) {
		return owner->tilesInRange();
	}
	return std::vector<LogicTile*>();
}
