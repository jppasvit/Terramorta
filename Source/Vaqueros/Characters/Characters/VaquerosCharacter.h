// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <vector>
#include "Vaqueros/GameController/VaquerosGameManager.h"
#include "VaquerosCharacter.generated.h"



class UGunManager;
class LogicTile;

UCLASS()
class VAQUEROS_API AVaquerosCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVaquerosCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when vaquero take damage
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Compare the position of an vaquero with a given error
	bool CompareVaquerosPosition(const FVector& newlocation, int error);

	// Change _realenergy 
	bool modifyEnergy(int amount);

	// New vaquero location
	FVector _newLocation;

	// New vaquero locations
	TArray<FVector> _newLocations;

	// Vaquero Mesh component (Already added in BP)
	//UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _meshComp;

	// Vaquero material (Already added in BP)
	//UPROPERTY(EditAnywhere)
	UMaterialInterface* _storedMaterial;

	// Manager for weapons
	UPROPERTY(EditAnywhere)
	UGunManager* _gunManagerComponent;

	// Real Vaquero energy
	//UPROPERTY(VisibleAnywhere)
	int _realenergy;

	// Vaquero energy
	UPROPERTY(EditAnywhere)
	int _energy = 100;

	// Vaquero life
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _hp = 100;

	// Max vaquero life
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _maxhp = 600;

	// Flag for put the vaquero in initial position
	UPROPERTY(EditAnywhere)
	bool _putOnInitialPosition = false;

	// X position (_putOnInitialPosition)
	UPROPERTY(EditAnywhere)
	int _xPosition = 0;

	// Y position (_putOnInitialPosition)
	UPROPERTY(EditAnywhere)
	int _yPosition = 0;

	// Determines if vaquero is an enemy or not
	UPROPERTY(EditAnywhere)
	bool _enemy = false;

	// Allowed motion error
	UPROPERTY(EditAnywhere)
	int _errorInMove = 9;

	// Speed in move
	UPROPERTY(EditAnywhere)
	float _speedInMove = 250.0;

	// Speed in rotation
	UPROPERTY(EditAnywhere)
	float _speedInRotation = 10.0;

	// Glow material
	UPROPERTY(EditAnywhere)
	UMaterialInterface* _glowMaterial;

	// Normal material
	UPROPERTY(EditAnywhere)
	UMaterialInterface* _normalMaterial;

	// Animation component
	UPROPERTY(EditAnywhere)
	UAnimationAsset *_animMovement;
	
	// Set flag _turnCompleted to value
	void SetTurnCompleted(bool value);

	// Return _turnCompleted
	bool IsTurnCompleted();

	// Determines if vaquero can do an action ("Attack", "Move")
	bool CanDoAction(const FString & action,  LogicTile * toLocation = nullptr );

	// Determines if vaquero can do an action
	bool CanDoAction();

	// Increase counter actions in one, and return it
	int IncreaseCounterActions();

	// Reset the actions counter
	void ResetCounterActions();

	// Return the actions counter
	int GetCounterActions();

	// Return _moveInTurnCompleted
	UFUNCTION(BlueprintCallable)
	bool IsMoveInTurnCompleted();

	// Return _attackInTurnCompleted
	UFUNCTION(BlueprintCallable)
	bool IsAttackInTurnCompleted();

	// Set flag _moveInTurnCompleted to value
	void SetMoveInTurnCompleted(bool value);

	// Set flag _attackInTurnCompleted to value
	void SetAttackInTurnCompleted(bool value);

	// Set flag _numMovements to value
	void SetNumMovements(int value);

	// Return _numMovements
	int GetNumMovements();

	// Return if can value movements
	bool CanDoMovements(int value);

	// Set vaquero old position
	void SetOldPositionMember(int xpos, int ypos, int oldrange);

	// Return vaquero old position
	int GetOldPostionMember(int index);

	// Return vaquero location Logic Tile 
	LogicTile * GetVaqueroLocation();
	
	//Dispara hacia Location
	virtual bool makeShot(FVector Location);

	//Devuelve objetivos a rango
	virtual std::vector<LogicTile*> objetivesInRange(LogicTile* tile = nullptr);

	//Devuelve las casillas en rango
	virtual std::vector<LogicTile*> tilesInRange(LogicTile* tile = nullptr);

	// Set new locations (_newLocations)
	void SetNewLocations(std::vector<LogicTile*> locations);

	// Set new locations (_newLocations)
	void SetNewLocations(TArray<FVector> locations);

	// Moves vaquero to tile if it is possible
	void MoveToTile(LogicTile * tile);

	// Determines if the vaquero movement is over
	bool IsMovementFinalized();

	// Reset cool down
	void ResetCoolDown();

	// Determines if vaquero glows or not
	void GlowCharacter(bool value);

	//void SetGameIndex(int index) { GameIndex = index; }

	//int GetGameIndex() { return GameIndex; }

	// Moves vaquero to tile if it is possible
	void MoveVaquerosCharToActor(LogicTile * toTile);

	// Attacks to tile if it is possible
	void Attack(LogicTile * toTile);

	// Show tiles in range
	void ShowTilesInRange(bool mode);

	// Show tiles for attack
	void ShowTilesForAttack(bool mode);

	// Show tiles for in range for attack
	void ShowTilesInRangeForAttack(bool mode);

	// Show all tiles (decals)
	void ShowAllTiles(bool mode);

	// Return name of character (_nameCharacter)
	FString GetNameCharacter() { return _nameCharacter; }

	// Set _nameCharacter 
	void SetNameCharacter(FString namecharacter) { _nameCharacter = namecharacter; }

	// Get vaquero life
	int GetHp() { return _hp; }

	// Set vaquero life
	void SetHp(int hp);

	// Modifies vaquero life
	void AddHp(int num);

	// Return _maxhp
	int GetMaxHp() { return _maxhp; }

	// Set max hp
	void SetMaxHp(int maxhp) { _maxhp = maxhp; }

	// Updates animation variables
	void UpdateAnimationVariables(bool &stop, bool &rotateLeft, bool &rotateRight, bool &receiveDamage, bool &shootAnim, AActor* &objetiveToShoot, bool &dieAnim, bool &turnAround);

	// Prepare animation shoot
	UFUNCTION(BlueprintCallable)
	void prepareAnimationShoot(AActor* objetive);

	// Prepare receive damage
	UFUNCTION(BlueprintCallable)
		void prepareReceiveDamage();

	// Ended receive damage
	UFUNCTION(BlueprintCallable)
	void endedReceiveDamage() { _receiveDamage = false; }

	// Ended rotation
	UFUNCTION(BlueprintCallable)
	void endedRotation() { _bRotateLeft = false; _bRotateRight = false; _turnAround = false; }

	// Ended shoot
	UFUNCTION(BlueprintCallable)
	void endedShoot() { _shootAnim = false; }

	// Update hud
	UFUNCTION(BlueprintCallable)
	void updateHud();

private:

	// Flag that determines if turn is completed
	bool _turnCompleted = false;

	// Counter actions
	int _counterActions = 0;

	// Flag that determines move action is completed
	bool _moveInTurnCompleted = false;

	// Flag that determines if attack action is completed
	bool _attackInTurnCompleted = false;

	// Num of movements allowed
	UPROPERTY(EditAnywhere)
	int _numMovements = 5;

	int _oldPosition[3] = { -1,-1,-1 }; // 0 -> x position, 1 -> y position, 2 -> Range

	//int GameIndex;

	// Id name of character
	UPROPERTY(EditAnywhere)
	FString _nameCharacter;

	// Vaqueros Game Manager instance
	VaquerosGameManager * vgminstance = VaquerosGameManager::GetInstance();

	// Vaqueros game mode
	AVaquerosGameMode * gamemode;

	//_isMoving is true when player is moving, false otherwise
	bool _isMoving = false;

	// Determines if vaquero is already in a tile
	bool _alreadyInTile = false;


	UPROPERTY(EditAnywhere)
	bool _bStop = true;
	UPROPERTY(EditAnywhere)
	bool _bRotateLeft = false;
	UPROPERTY(EditAnywhere)
	bool _bRotateRight = false;
	UPROPERTY(EditAnywhere)
	AActor* _objetiveToShoot = nullptr;
	UPROPERTY(EditAnywhere)
	bool _shootAnim = false;
	UPROPERTY(EditAnywhere)
	bool _receiveDamage = false;
	UPROPERTY(EditAnywhere)
	bool _dieAnim = false;
	UPROPERTY(EditAnywhere)
	bool _turnAround = false;
};
