// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBaseComponent.h"
#include "MatrixLogic/Matriz.h"
#include "MatrixLogic/LogicTile.h"
#include "Characters/Characters/VaquerosCharacter.h"
// Sets default values for this component's properties
UGunBaseComponent::UGunBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


bool UGunBaseComponent::makeShot(LogicTile * source, LogicTile * destination)
{
	/*if (!ammo)
		return false;*/
	int dirX = destination->getLogicalX() - source->getLogicalX();
	int dirY = destination->getLogicalY() - source->getLogicalY();
	if ((dirX != 0 && dirY != 0) || (dirX == 0 && dirY == 0))
		return false;
	int incX = 0;
	int incY = 0;
	int dist = 0;
	int objX = source->getLogicalX();
	int objY = source->getLogicalY();
	LogicTile* objetive = nullptr;
	incX = getIncr(dirX);
	incY = getIncr(dirY);
	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	do {
		objX += incX;
		objY += incY;

		objetive = matrix->GetTile(objX, objY);
		++dist;

	} while (dist <= range && objetive && destination != objetive && !objetive->_occupied);
	if (dist <= range && objetive &&objetive->getOccupant()) {
		FDamageEvent dmg;
		objetive->getOccupant()->TakeDamage(damage, dmg, nullptr, GetOwner());
		//--ammo;
		return true;
	}
	return false;
}

std::vector<LogicTile*> UGunBaseComponent::tilesAffected(LogicTile * source)
{
	std::vector<LogicTile*> aux = std::vector<LogicTile*>();
	aux.push_back(source);
	return aux;
}

std::vector<LogicTile*> UGunBaseComponent::objetivesInRange(LogicTile * source, AVaquerosCharacter * shooter)
{
	std::vector<LogicTile*> objetives = std::vector<LogicTile*>();
	AActor* aux = searchObjetive(source, 1, 0, shooter);
	AVaquerosCharacter* objetive = Cast<AVaquerosCharacter>(aux);
	if (objetive && objetive != shooter && objetive->_enemy != shooter->_enemy) {
		objetives.push_back(objetive->GetVaqueroLocation());
	}
	aux = searchObjetive(source, -1, 0, shooter);
	objetive = Cast<AVaquerosCharacter>(aux);
	if (objetive && objetive != shooter && objetive->_enemy != shooter->_enemy) {
		objetives.push_back(objetive->GetVaqueroLocation());
	}
	aux = searchObjetive(source, 0, 1, shooter);
	objetive = Cast<AVaquerosCharacter>(aux);
	if (objetive && objetive != shooter && objetive->_enemy != shooter->_enemy) {
		objetives.push_back(objetive->GetVaqueroLocation());
	}
	aux = searchObjetive(source, 0, -1, shooter);
	objetive = Cast<AVaquerosCharacter>(aux);
	if (objetive && objetive != shooter && objetive->_enemy != shooter->_enemy) {
		objetives.push_back(objetive->GetVaqueroLocation());
	}
	return objetives;
}

std::vector<LogicTile*> UGunBaseComponent::tilesInRange(LogicTile * source)
{
	std::vector<LogicTile*> tilesInRange = std::vector<LogicTile*>();

	searchTilesInRange(source, 1, 0, tilesInRange);
	searchTilesInRange(source, -1, 0, tilesInRange);
	searchTilesInRange(source, 0, 1, tilesInRange);
	searchTilesInRange(source, 0, -1, tilesInRange);

	return tilesInRange;
}

bool UGunBaseComponent::canShoot(LogicTile * source, LogicTile * destination)
{
	int dirX = destination->getLogicalX() - source->getLogicalX();
	int dirY = destination->getLogicalY() - source->getLogicalY();
	if ((dirX != 0 && dirY != 0) || (dirX == 0 && dirY == 0))
		return false;
	int incX = 0;
	int incY = 0;
	int dist = 0;
	int objX = source->getLogicalX();
	int objY = source->getLogicalY();
	LogicTile* objetive = nullptr;
	incX = getIncr(dirX);
	incY = getIncr(dirY);
	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	do {
		objX += incX;
		objY += incY;

		objetive = matrix->GetTile(objX, objY);
		++dist;

	} while (dist <= range && objetive && destination != objetive && !objetive->_occupied);
	if (dist <= range && objetive &&objetive->getOccupant()) {
		return true;
	}
	return false;
}

// Called when the game starts
void UGunBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ammo = maxAmmo;
}


// Called every frame
void UGunBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor * UGunBaseComponent::searchObjetive(LogicTile * source, int incrX, int incrY, AVaquerosCharacter * shooter)
{
	AActor* objetive = nullptr;
	int x = source->getLogicalX() + incrX, y = source->getLogicalY() + incrY;
	int distance = 1;
	Matriz * actualMatrix = VaquerosGameManager::GetInstance()->GetMatrix();
	while (actualMatrix->GetTile(x, y) && distance < range) {
		if (actualMatrix->GetTile(x, y)->getOccupant() && actualMatrix->GetTile(x, y)->getOccupant() != shooter) {
			break;
		}
		++distance;
		x += incrX;
		y += incrY;
	}
	if (actualMatrix->GetTile(x, y)) {
		objetive = actualMatrix->GetTile(x, y)->getOccupant();
	}

	return objetive;
}

void UGunBaseComponent::searchTilesInRange(LogicTile * source, int incrX, int incrY, std::vector<LogicTile*>& tilesInRange)
{
	int x = source->getLogicalX() + incrX, y = source->getLogicalY() + incrY;
	int distance = 1;
	Matriz * actualMatrix = VaquerosGameManager::GetInstance()->GetMatrix();
	while (actualMatrix->GetTile(x, y) && (!actualMatrix->GetTile(x, y)->_occupied) && distance <= range) {
		++distance;
		tilesInRange.push_back(actualMatrix->GetTile(x, y));
		x += incrX;
		y += incrY;
	}
}

int UGunBaseComponent::getIncr(int dir)
{
	int inc = 0;
	if (dir < 0)
		inc = -1;
	else if (dir > 0)
		inc = 1;
	return inc;
}

