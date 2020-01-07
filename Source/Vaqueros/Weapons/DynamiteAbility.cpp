// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamiteAbility.h"


#include "Characters/Characters/Wall.h"
#include "MatrixLogic/Matriz.h"
#include "Characters/Characters/VaquerosCharacter.h"


int getIncr(int dir) {
	int inc = 0;
	if (dir < 0)
		inc = -1;
	else if (dir > 0)
		inc = 1;
	return inc;
}

bool UDynamiteAbility::ExecuteAbility(LogicTile * source, LogicTile * destination)
{
	//Destination must be empty or occuped by VaquerosCharacter
	if (!destination
		|| (destination->_occupied && destination->getOccupant())
		|| !destination->getOccupant()->IsA(AVaquerosCharacter::StaticClass())) {
			return false;
	}
	int dirX = destination->getLogicalX() - source->getLogicalX();
	int dirY = destination->getLogicalY() - source->getLogicalY();
	if ((dirX != 0 && dirY != 0) || (dirX == 0 && dirY == 0))
		return false;
	int incX = 0;
	int incY = 0;
	int dist = 0;
	int objX = source->getLogicalX();
	int objY = source->getLogicalY();
	LogicTile* intermediate = nullptr;
	incX = getIncr(dirX);
	incY = getIncr(dirY);
	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	bool valido = true;
	do {
		objX += incX;
		objY += incY;

		intermediate = matrix->GetTile(objX, objY);
		++dist;
		if (intermediate&&intermediate->getOccupant()) {
			AWall* aux = Cast<AWall>(intermediate->getOccupant());
			if (aux)
				valido = aux->_throwOver;
		}
	} while (dist <= _range && intermediate && destination != intermediate && valido);
	
	if (destination == intermediate) {
		std::vector<LogicTile*> tilesToDamage = tilesAffected(destination);
		for (LogicTile* tile : tilesToDamage) {
			FDamageEvent dmg;
			tile->getOccupant()->TakeDamage(_damage, dmg, nullptr, source->getOccupant());
		}
	}
	return true;
}

std::vector<LogicTile*> UDynamiteAbility::tilesAffected(LogicTile * source)
{
	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	int centreX = source->getLogicalX(), centreY= source->getLogicalY();
	std::vector<LogicTile*> aux = std::vector<LogicTile*>();
	if (matrix->GetTile(centreX, centreY)) {
		if (matrix->GetTile(centreX, centreY)->getOccupant())
			aux.push_back(matrix->GetTile(centreX, centreY));
	}
	if (matrix->GetTile(centreX - 1, centreY)) {
		if (matrix->GetTile(centreX - 1, centreY)->getOccupant())
			aux.push_back(matrix->GetTile(centreX-1, centreY));
	}
	if (matrix->GetTile(centreX + 1, centreY)) {
		if (matrix->GetTile(centreX + 1, centreY)->getOccupant())
			aux.push_back(matrix->GetTile(centreX + 1, centreY));
	}
	if (matrix->GetTile(centreX, centreY - 1)) {
		if (matrix->GetTile(centreX, centreY - 1)->getOccupant())
			aux.push_back(matrix->GetTile(centreX, centreY - 1));
	}
	if (matrix->GetTile(centreX, centreY + 1)) {
		if (matrix->GetTile(centreX, centreY + 1)->getOccupant())
			aux.push_back(matrix->GetTile(centreX, centreY + 1));
	}
	
	return aux;
}