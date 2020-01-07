// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VaquerosGameMode.generated.h"
class UUserWidget;
class AVaquerosCharacter;
class AVaquerosCameraActor;
/**
 * 
 */
UCLASS()
class VAQUEROS_API AVaquerosGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AVaquerosGameMode();
	void BeginPlay() override;
	void StartPlay() override;

	// Update character (Update image, UpdateCharacter BP) 
	void UpdateCharacter(AVaquerosCharacter* actualCharacter);

	// Update hud for change turn (ChangeTurn BP)
	void ChangeTurn(bool enemyTurn);

	// Pass sub turn of player
	UFUNCTION(BlueprintCallable)
	void PassSubTurn();
	
	// Pass turn of players
	UFUNCTION(BlueprintCallable)
	void PassTurn();

	// Determines if HoverMoveDecals can be shown
	UFUNCTION(BlueprintCallable)
	bool CanShowHoverMoveDecal(AActor * shadow);

	// Determines if HoverShootDecals can be shown
	UFUNCTION(BlueprintCallable)
	bool CanShowHoverShootDecal(AActor * shadow);

	// Determines if ShootingRange can be shown when cursor are over players
	UFUNCTION(BlueprintCallable)
	void ShowShootingRangeCursorOver(AActor * shadow, bool mode);

	// Returns current character (selected player)
	UFUNCTION(BlueprintCallable)
	AVaquerosCharacter* GetCurrentCharacter();

	// Returns character by CharacterName
	UFUNCTION(BlueprintCallable)
	AVaquerosCharacter* GetVCharacterByCharacterName(FString name);

	// Sets player in turn by CharacterName
	UFUNCTION(BlueprintCallable)
	bool SetPlayerInTurnByCharacterName(FString name);

	// Determines if turn of character is completed
	UFUNCTION(BlueprintCallable)
	bool IsCharacterTurnCompleted(AVaquerosCharacter * character);

	// Determines if character actions are completed
	UFUNCTION(BlueprintCallable)
	bool IsCharacterActionsCompleted(AVaquerosCharacter * character);

	// Notifies changes of character attributes to the interface
	UFUNCTION(BlueprintCallable)
	void NotifyChangesOfAttributes(AVaquerosCharacter * character);

	// Notifies changes of all character attributes to the interface
	void CheckNCOAAllPlayers();

	// Set camera as main
	void setCamera(AVaquerosCameraActor* camera);

	// Returns main camera
	UFUNCTION(BlueprintCallable)
	AVaquerosCameraActor* getCamera(){ return _VaquerosCamera; }
	
	bool _dayMode;

	bool _dayModeCorrect = false;;

	// Sets day mode
	void setDayMode(bool dayMode);

	// Returns day mode
	UFUNCTION(BlueprintCallable)
	bool GetDayMode() { return _dayMode; }

	// Returns day mode correct
	UFUNCTION(BlueprintCallable)
	bool GetDayModeCorrect() { return _dayModeCorrect; }

private:
	UClass * _VaquerosHudClass;
	UUserWidget*  _VaquerosHud;
	AVaquerosCameraActor* _VaquerosCamera;

};
