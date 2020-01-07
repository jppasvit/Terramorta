// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameController/VaquerosGameManager.h"
#include "VaquerosGameInstance.generated.h"

class BasicWeapon;
class UUserWidget;
class AVaquerosCharacter;
/**
 * 
 */
UCLASS()
class VAQUEROS_API UVaquerosGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UVaquerosGameInstance();
private:
	VaquerosGameManager* _gameManager = VaquerosGameManager::GetInstance();
	int _currentMap = 0;
	int _currentSubLevel = 0;
	UClass * _victoryMenu;
	UClass * _defeatMenu;
	TSet<FString> MapFiles;
	TMap<FString,int> HPCharacters;
	TMap<FString, bool> charactersAlive;

public:
	UFUNCTION(BlueprintCallable)
	void startMap(int map, int subLevel);
	void notifyEndGame(bool result);
	UFUNCTION(BlueprintCallable)
	void backToWelcomeMenu();
	UFUNCTION(BlueprintCallable)
	void startNextMap();
	UFUNCTION(BlueprintCallable)
	void restartMap();
	void SetPlayersHP(AVaquerosCharacter** characters, int num);
	void UpdateHPCharacter(AVaquerosCharacter* character, int hp);
	void UpdateAllHPCharacters(AVaquerosCharacter** characters, int num);
	void NotifyDead(AVaquerosCharacter* character);

private:
	void startNewRound(AVaquerosCharacter** characters, int num);
	void startSublevel(AVaquerosCharacter** characters, int num);
	void saveHPCharacters();
	
};
