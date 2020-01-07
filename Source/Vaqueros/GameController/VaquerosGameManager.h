// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"

class Matriz;
class AVaquerosCharacter;
class AVaquerosGameMode;

/**
 * 
 */
class VAQUEROS_API VaquerosGameManager 
{
public:
	// Returns same instance of VaquerosGameManager
	static VaquerosGameManager* GetInstance();

	// Delete instance of VaquerosGameManager
	static void ReleaseInstance();
	
	// General

	// Reset variables
	void SetToNull();

	// Return matrix
	Matriz* GetMatrix();

	// Set matrix
	void SetMatrix(Matriz* matrix);

	// Set Vaqueros characters in matrix of players and enemies
	void SetVCharactersInGame(TArray<AActor*> characters);

	// Get Vaqueros Character by character name
	AVaquerosCharacter* GetVCharacterByCharacterName(FString name);

	// Flag to allow SetPlayerInTurnByCharacterName in Blueprints
	bool CanBlueprintSetPlayerInTurnByCharacterName = true;

	// Players

	// Set ACharacter to _actualCharacter
	void SetCurrentCharacter(ACharacter* actualCharacter);

	// Get current character (_actualCharacter)
	ACharacter* GetCurrentCharacter();

	// Set player to player array
	void SetPlayer(AVaquerosCharacter* player);

	// Set players to player array
	void SetPlayers(AVaquerosCharacter** players);

	// Get player position by id
	AVaquerosCharacter* GetPlayer(int pos);

	// Get players array
	AVaquerosCharacter** GetPlayers();

	// Get num players
	int GetNumPlayers();

	// Get index player by player of array
	int GetIndexPlayer(AVaquerosCharacter * player);

	// Set num max of players
	void SetNumMaxPlayers(int maxNumPlayers);

	// Returns player in turn
	AVaquerosCharacter* GetPlayerInTurn();

	// Set player in turn
	int SetPlayerInTurn(AVaquerosCharacter* player);

	// Returns index of player in turn
	int GetIndexOfPlayerInTurn();

	// Set index of player in turn
	void SetIndexOfPlayerInTurn(int index);

	// Set Vaqueros Character to current character (_actualCharacter) and in turn
	void SetCurrentCharacterAndInTurn(AVaquerosCharacter* player);

	// Enemies

	// Set maximum number of enemies
	void SetNumMaxEnemies(int maxNumEnemies);

	// Set enemy in enemy array
	void SetEnemy(AVaquerosCharacter* enemy);

	// Returns enemy by position in enemies array
	AVaquerosCharacter* GetEnemy(int pos);

	// Returns enemies array
	AVaquerosCharacter** GetEnemies();

	// Return number of enemies
	int GetNumEnemies();

	// Return index of enemy by enemy in array
	int GetIndexEnemy(AVaquerosCharacter * enemy);

	// Returns enemy in turn
	AVaquerosCharacter* GetEnemyInTurn();

	// Sets enemy in turn
	int SetEnemyInTurn(AVaquerosCharacter* player);

	// Get index of enemy in turn
	int GetIndexOfEnemyInTurn();

	// Set index of enemy in turn
	void SetIndexOfEnemyInTurn(int index);
	

	// Turns

	// Start turn of AVaquerosCharacter
	void StartTurn(AVaquerosCharacter* player);

	// Starts turn of players
	void StartTurnPlayers();

	// End turn player
	void EndTurn(AVaquerosCharacter* player);

	// Finish movement of turn
	void EndMovementTurn(AVaquerosCharacter* player);

	// End attack of turn
	void EndAttackTurn(AVaquerosCharacter* player);

	// End trun of players
	void EndTurnPlayers();

	// Returns if turn of players is ended
	bool IsPlayersTurnEnded();

	// Determines if turn of players can be passed
	bool CanPassTurnToPlayer(AVaquerosCharacter* player);
	
	// Returns actions per turn
	int GetActionsPerTurn();

	// Sets actions per turn
	void SetActionsPerTurn(int value);

	// Determines if turn of character are completed
	bool IsCharacterTurnCompleted(AVaquerosCharacter * character);

	// Determines if actios of character are completed
	bool IsCharacterActionsCompleted(AVaquerosCharacter * character);

	// Check global turn of all players, and end their turns
	void CheckGlobalPlayersTurn();
	
	// Check character turn
	bool CheckPlayerTurn(AVaquerosCharacter * character);

	// Check characters turn: True if character can't do any action, false otherwise
	bool CheckVaqueroTurn(AVaquerosCharacter * character);

	// NextEnemyTurn: Update enemy in turn one by one.
	// Return true if new enemy in turn has been updated successfully, return false otherwise
	bool NextEnemyTurn();

	// Returns if enemy is in turn
	bool IsEnemyInTurn(AVaquerosCharacter * enemy);

	// Returns if enemies turn is ended
	bool IsEnemiesTurnEnded();

	// End turn of enemies
	void EndTurnEnemies();

	// Start turn of enemies
	void StartTurnEnemies();

	// Returns if enemy turn ended
	bool EnemyTurnEnded(AVaquerosCharacter * enemy);

	// Finish enemy turn
	bool FinishEnemyTurn(AVaquerosCharacter * enemy);

	// Checks enemy turn
	bool EnemyCheckTurn(AVaquerosCharacter * enemy);

	// Complete move action of player turn
	void CompleteMoveInTurn(AVaquerosCharacter* player);

	// Complete attack action of player turn
	void CompleteAttackInTurn(AVaquerosCharacter* player);

	// Destroy VaquerosCharacter
	void DestroyVaquerosCharacter(AVaquerosCharacter* vcharacter, bool anim = false);

	// Victory conditions

	// Returns if players win the game
	bool HavePlayersWon();

	// Returns if enemies win the game
	bool HaveEnemiesWon();

	// Pendig to die

	// Set character pendig to die
	void SetCharacterPendingToDie(AVaquerosCharacter * character);

	// Returns character pending to die
	AVaquerosCharacter * GetCharacterPendigToDie();

	// Returns if character is pendig to die
	bool IsCharacterPendigToDie(AVaquerosCharacter * character);

	// LOG
	void LogCharacter(AVaquerosCharacter * character);
	void LogCharacters();

	VaquerosGameManager& operator=(const VaquerosGameManager&) = delete;  // Disallow copying
	VaquerosGameManager(const VaquerosGameManager&) = delete;

private:
	VaquerosGameManager();
	~VaquerosGameManager();
	static VaquerosGameManager* instance;
	ACharacter* _actualCharacter;
	Matriz* _matrix;

	//Players
	AVaquerosCharacter** _players;
	int _numplayers = 0;
	int _maxNumPlayers = -1;
	int _indexOfPlayerInTurn = -1;
	
	//Enemies
	AVaquerosCharacter** _enemies;
	int _numenemies = 0;
	int _maxNumEnemies = -1;
	int _indexOfEnemyInTurn = -1;

	// Action per turn (Enemy and Player)
	int _actionsPerTurn = 2;

	// Pendig to kill
	AVaquerosCharacter * characterPendingToDie;
};
