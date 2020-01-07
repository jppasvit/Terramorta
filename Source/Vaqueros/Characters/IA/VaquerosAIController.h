// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include <vector>
#include "MatrixLogic/LogicTile.h"
#include "Characters/Characters/VaquerosCharacter.h"
using namespace std;
#include "VaquerosAIController.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API AVaquerosAIController : public AAIController
{
	GENERATED_BODY()

private:
	vector<LogicTile*> _tilesPath;

public:

	virtual void BeginPlay() override;

	// Determines if the vaquero movement is over
	bool CharacterMovementFinalized();

	// Updates attack objetive 
	bool UpdateTarget();

	// Determines if an objective is valid
	bool isTargetValid();

	UPROPERTY(EditAnywhere)
	float _movementSpeed = .5f;

	// Searches random tile point. True if tile point is found; false otherwise.
	UFUNCTION(BlueprintCallable, Category = "Task")
	bool GetRandomTilePoint();

	// Searches best tile point (FindPositionWithMoreEnemies). True if tile point is found; false otherwise.
	UFUNCTION(BlueprintCallable, Category = "Task")
	bool GetBestTilePoint();

	// Searches tile with closest enemy. True if tile point is found; false otherwise.
	UFUNCTION(BlueprintCallable, Category = "Task")
	bool GetClosestEnemyTilePoint();

	// Searches closest shot point. True if tile point is found; false otherwise.
	UFUNCTION(BlueprintCallable, Category = "Task")
	bool GetClosestShotPoint();

	UFUNCTION()
	void GoToRandomTilePoint();

	// Update tile for moving
	UFUNCTION()
	void UpdateTileToMove();

	// Update target to shoot
	UFUNCTION()
	bool UpdateObjetiveToShoot();

	// Move pawn to target tile
	UFUNCTION()
	void TaskMoveToTile();

	// Shoots to target location if it is possible
	UFUNCTION()
	bool TaskShootObjetive();

	// Returns if is turn of pawn
	UFUNCTION()
	bool IsMyTurn();

	// Involves change turn operations 
	UFUNCTION()
	void TurnEnded();

	// Finish turn of pawn
	UFUNCTION()
	void TurnFinished();

	// End movement action of pawn
	UFUNCTION()
	void MovementTurnEnded();

	// End attack action  of pawn
	UFUNCTION()
	void AttackTurnEnded();

	// Check turn of pawn
	UFUNCTION()
	bool CheckTurn();

	// Select action 
	UFUNCTION()
	bool SelectAction();


	//Devuelve el player más cercano
	AVaquerosCharacter* getClosestPlayer(AVaquerosCharacter* vaquerosPawn, Matriz* matrix);


};
