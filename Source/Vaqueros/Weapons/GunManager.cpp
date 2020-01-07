// Fill out your copyright notice in the Description page of Project Settings.

#include "GunManager.h"
#include "Engine.h"
#include "VaquerosGameInstance.h"
#include "GunBaseComponent.h"
//#include "BasicAbility.h"


// Sets default values for this component's properties
UGunManager::UGunManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}



// Called when the game starts
void UGunManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
	Gun = Cast<UGunBaseComponent>(GetOwner()->GetComponentByClass(UGunBaseComponent::StaticClass()));
	//Ability = Cast<UBasicAbility>(GetOwner()->GetComponentByClass(UBasicAbility::StaticClass()));
}


// Called every frame
void UGunManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGunManager::makeShot(LogicTile * source, LogicTile * destination)
{
	if (Gun && Gun->canShoot() && Gun->canShoot(source, destination)) {
		Gun->makeShot(source, destination);
		return true;
	}
	return false;
}

bool UGunManager::canShoot()
{
	if (Gun) {
		return Gun->canShoot();
	}
	return false;
}

bool UGunManager::canShoot(LogicTile * source, LogicTile * destination)
{
	if (Gun) {
		return Gun->canShoot(source, destination);
	}
	return false;
}

std::vector<LogicTile*> UGunManager::objetivesInRange(LogicTile * source, AVaquerosCharacter * shooter)
{
	if (Gun && Gun->canShoot()) {
		return Gun->objetivesInRange(source, shooter);
	}
	return std::vector<LogicTile*>();
}

std::vector<LogicTile*> UGunManager::tilesInRange(LogicTile* source)
{
	if (Gun)
		return Gun->tilesInRange(source);
	return std::vector<LogicTile*>();
}

void UGunManager::reload()
{
	if (Gun)
		Gun->Reload();
}

void UGunManager::myTick()
{
}
/*
bool UGunManager::ExecuteAbility(LogicTile * source, LogicTile * destination)
{
	if (Ability)
		return Ability->ExecuteAbility(source, destination);
	return false;
}

std::vector<LogicTile*> UGunManager::tilesInAbilityRange(LogicTile * source)
{
	if (Ability)
		return Ability->tilesAffected(source);
	return std::vector<LogicTile*>();
}
*/