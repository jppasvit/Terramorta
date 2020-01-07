// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatrixLogic/LogicTile.h"
#include "GameFramework/PlayerController.h"
#include "Vaqueros/GameController/VaquerosGameManager.h"
#include "VaquerosPlayerController.generated.h"

/**
 *
 */
class AVaquerosCharacter;
class AVaquerosGameMode;

UCLASS()
class VAQUEROS_API AVaquerosPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVaquerosPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Capture any click to generate orders for the game
	void OnClickAnyWhere();

	// Pass turn of selected player
	void PassTurn();

	// Checks if its possible to click to generate orders
	bool CheckCanClickAnyWhere();

	// Sets enemy for selectedEnemy
	void SetSelectedEnemy(AVaquerosCharacter * enemy);

	// Returns selected enemy
	AVaquerosCharacter * GetSelectedEnemy();

	// Sets isTilesSelectedEnemyIlluminated
	void SetIsTilesSelectedEnemyIlluminated(bool var);

	// Returns isTilesSelectedEnemyIlluminated
	bool IsTilesSelectedEnemyIlluminated();

	// Returns isSelectedEnemyDestroyed
	bool IsSelectedEnemyDestroyed();

	// Sets isSelectedEnemyDestroyed
	void SetIsSelectedEnemyDestroyed(bool var);

private:

	// VaquerosGameManager instance
	VaquerosGameManager * vgminstance = VaquerosGameManager::GetInstance();

	AVaquerosGameMode * gamemode;

	// Enemy selected 
	AVaquerosCharacter * selectedEnemy;

	// Flag to know if selected enemy has tiles illuminated
	bool isTilesSelectedEnemyIlluminated;

	// Flag to know of selected enemy is destroyed
	bool isSelectedEnemyDestroyed = false;
};
