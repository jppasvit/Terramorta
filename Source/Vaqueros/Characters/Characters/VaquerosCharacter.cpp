// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Characters/VaquerosCharacter.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
//#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameController/VaquerosGameManager.h"
#include "MatrixLogic/Matriz.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include <string>
#include "Weapons/GunManager.h"
#include "MatrixLogic/LogicTile.h"
#include "Utils/Pathfinding.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Vaqueros/Characters/Controllers/VaquerosPlayerController.h"
#include "VaquerosGameMode.h"
#include <vector>
#include "VaquerosGameInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
AVaquerosCharacter::AVaquerosCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_realenergy = _energy;
	
	_gunManagerComponent = CreateDefaultSubobject<UGunManager>(TEXT("UGunManager"));
	//_gunManagerComponent->AddToRoot();
}

// Called when the game starts or when spawned
void AVaquerosCharacter::BeginPlay()
{
	Super::BeginPlay();
	//_gunManagerComponent->init(1);
	//_gunManagerComponent->setWeapon(0, 0);
	// When game start puts _maxenergy as _energy
	_realenergy = _energy;
	/*if (_putOnInitialPosition)
	{
		Matriz* matrix = vgminstance->GetMatrix();
		FVector toPosition = matrix->locationTile(_xPosition, _yPosition);
		this->SetActorLocation(FVector(toPosition.X, toPosition.Y, this->GetActorLocation().Z));
		matrix->GetTile(matrix->getCoor(toPosition.X), matrix->getCoor(toPosition.Y))->SetOccupant(this);
	}*/
	// Register player in array if it isn't an enemy
	/*if (!_enemy) {
		vgminstance->SetPlayer(this);
	}
	else {
		vgminstance->SetEnemy(this);
	}*/
	gamemode = Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	/*if(!_enemy){
		if(!gamemode->HasActorBegunPlay())
		{
			gamemode->DispatchBeginPlay();
		}
		gamemode->NotifyChangesOfAttributes(this);
	}*/
}

float AVaquerosCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Damage Taken"));


	FVector direction = (DamageCauser->GetActorLocation() - GetActorLocation());
	direction.Normalize();
	float dotValue = FVector::DotProduct(direction, GetActorRotation().Vector());
	float AimAtAngle = acosf(dotValue);
	if (!FMath::IsNearlyZero(AimAtAngle, HALF_PI / 2)) {
		if (FMath::IsNearlyEqual(AimAtAngle, PI, HALF_PI / 2)) {
			_turnAround = true;
		}
		else {
			bool x = FMath::IsNearlyEqual(GetActorRightVector().X, direction.X, 0.5f);
			bool y = FMath::IsNearlyEqual(GetActorRightVector().Y, direction.Y, 0.5f);
			if (x&y) {
				_bRotateRight = true;
			}
			else {
				_bRotateLeft = true;
			}
		}
	}

	//_hp -= Damage;
	AddHp(-Damage);
	FEditorScriptExecutionGuard ScriptGuard;
	
	//std::string life = std::to_string(_hp);
	if (GetHp() <= 0) {
	//this->Destroy();
		if (!_enemy)
			//Notify GameInstance last _hp before die
			//Cast<UVaquerosGameInstance>(GetWorld()->GetGameInstance())->UpdateHPCharacter(this, _hp);
			Cast<UVaquerosGameInstance>(GetWorld()->GetGameInstance())->NotifyDead(this);

		vgminstance->EndTurn(this);
		gamemode->NotifyChangesOfAttributes(this);
		vgminstance->DestroyVaquerosCharacter(this, true);
		if(this->_enemy){
			AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
			playercontroller->SetIsSelectedEnemyDestroyed(true);
		}
	}
	//AMatriz* matrix = vgminstance->GetMatrix();
	//matrix->GetTile(matrix->getCoor(this->GetActorLocation().X), matrix->getCoor(this->GetActorLocation().Y))->SetOccupant(nullptr);
	return 0.0f;
}

// Called every frame
void AVaquerosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector auxLocation;

	if (_newLocations.Num() > 0 )  {
		_isMoving = true;
		auxLocation = this->GetActorLocation();
		if ((_newLocation.X == auxLocation.X && _newLocation.Y == auxLocation.Y) || _alreadyInTile)
		{
			_alreadyInTile = false;
			_newLocation = FVector(this->_newLocations[0].X, this->_newLocations[0].Y, this->GetActorLocation().Z);
			this->_newLocations.RemoveAt(0);
			_bStop = false;
			_bRotateLeft = false;
			_bRotateRight = false;
		}
		else if (_newLocation.X != this->_newLocations[0].X && _newLocation.Y != this->_newLocations[0].Y) {
			_bStop = false;
			_bRotateLeft = false;
			_bRotateRight = false;
			_newLocation = FVector(this->_newLocations[0].X, this->_newLocations[0].Y, this->GetActorLocation().Z);
			_alreadyInTile = false;
		}
			
	}

	if (_newLocation != FVector(0,0,0) && this->GetActorLocation() != _newLocation) {
		
		FVector direction = _newLocation - this->GetActorLocation();
		direction.Normalize();


		this->SetActorLocation(this->GetActorLocation() + (direction * _speedInMove * DeltaTime), true);
		
		//Cambiar Rotacion
		if ((_newLocation - this->GetActorLocation()).Size()>50) {
			FRotator actualRotation = GetActorRotation();
			FRotator objetiveRotation = direction.Rotation();
			/*float angle = UKismetMathLibrary::Atan2(objetiveRotation.Yaw, objetiveRotation.Roll) - UKismetMathLibrary::Atan2(actualRotation.Yaw, actualRotation.Roll);
			if (!FMath::IsNearlyEqual(angle, 0, HALF_PI / 2)) {
				if (FMath::IsNearlyEqual(angle, HALF_PI, HALF_PI/4)) {
					_bRotateRight = true;
					_bRotateLeft = false;
				}
				else {
					if (FMath::IsNearlyEqual(angle, -HALF_PI, HALF_PI / 4)) {
						_bRotateLeft = true;
						_bRotateRight = false;
					}
					else {
						_bRotateLeft = false;
						_bRotateRight = true;
					}
				}

			}else {
				_bRotateLeft=false;
				_bRotateRight=false;
			}
			
			objetiveRotation.Pitch = actualRotation.Pitch;*/
			SetActorRotation(FMath::RInterpTo(actualRotation, objetiveRotation, DeltaTime, _speedInRotation));
		}

		//

		//Comprueba si son iguales 
		if (CompareVaquerosPosition(_newLocation, _errorInMove)) {

			_alreadyInTile = true;

				//this->SetActorLocation(_newLocation);
				auxLocation = this->GetActorLocation();
				if(this->_newLocations.Num() == 0){
					_isMoving = false;
					_newLocation = FVector::ZeroVector;
					_bRotateLeft = false;
					_bRotateRight = false;
					if (!this->IsAttackInTurnCompleted() && !this->_enemy) {
						_shootAnim = false;
						this->ShowTilesForAttack(true);
						this->ShowTilesInRangeForAttack(true);
					}

					_bStop = true;

					if (vgminstance->CheckPlayerTurn(this)) {
						vgminstance->EndTurn(this);
						if (vgminstance->IsPlayersTurnEnded()) {
							gamemode->ChangeTurn(true);
							this->ShowTilesInRangeForAttack(false);
							// Cast<AVaquerosCharacter>(vgminstance->GetCurrentCharacter()) in this case, can be 'this'
							this->GlowCharacter(false);
						}
					}
					gamemode->NotifyChangesOfAttributes(this);
				}
		}
		
	}

	/*
		UE_LOG(LogTemp, Error, TEXT("MOVIENDOSE"));
		UE_LOG(LogTemp, Error, TEXT("-------------------"));
		UE_LOG(LogTemp, Error, TEXT("NEW LOCATION..."));
		UE_LOG(LogTemp, Error, TEXT("-------------------"));
		UE_LOG(LogTemp, Warning, TEXT("_newLocation: %s"), *_newLocation.ToString());

		UE_LOG(LogTemp, Error, TEXT("-------------------"));
		UE_LOG(LogTemp, Error, TEXT("ACTUAL ACTOR LOCATION..."));
		UE_LOG(LogTemp, Error, TEXT("-------------------"));
		UE_LOG(LogTemp, Warning, TEXT("GetActorLocation: %s"), *GetActorLocation().ToString());
	*/
}

// Called to bind functionality to input
void AVaquerosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)	
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#if WITH_EDITOR
void AVaquerosCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{

	FName propertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	//Change material
	if (_meshComp && (propertyName == GET_MEMBER_NAME_CHECKED(AVaquerosCharacter, _storedMaterial)))
	{
		_meshComp->SetMaterial(0, _storedMaterial);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, propertyName.GetPlainNameString());
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

bool AVaquerosCharacter::modifyEnergy(int amount)
{
	bool correct = true;
	int temp_energy = _realenergy + amount;

	if (temp_energy < 0 || temp_energy > _energy) {
		correct = false;
	}
	else {
		_realenergy = temp_energy;
	}

	return correct;
}

void AVaquerosCharacter::SetTurnCompleted(bool value)
{
	this->_turnCompleted = value;
}

bool AVaquerosCharacter::IsTurnCompleted()
{
	return this->_turnCompleted;
}

bool AVaquerosCharacter::CanDoAction(const FString& action ,  LogicTile * toLocation )
{
	Matriz* matrix = vgminstance->GetMatrix();

	FString actionlc = action.ToLower();

	if (actionlc == FString("move")) {

		if (this->IsMoveInTurnCompleted()) {
			return false;
		}

		if(toLocation){

			LogicTile *fromTile = GetVaqueroLocation();

			if (!toLocation->_walkable || toLocation->_occupied || _realenergy <= 0 || toLocation == fromTile){
				return false;
			}

			std::vector<LogicTile*> path = Pathfinding::FindPath(fromTile, toLocation, matrix);

			if (_newLocations.Num() > 0 || !CanDoMovements(path.size())){
				return false;
			}

			int totalcost = 0;
			for (LogicTile* tile : path) totalcost += tile->_cost;
			
			if (totalcost > _realenergy) {
				UE_LOG(LogTemp, Warning, TEXT("Not enough energy to move (return)"));
				return false;
			}

		}
		return vgminstance->GetActionsPerTurn() > this->_counterActions && !this->_moveInTurnCompleted;
	}else if(actionlc == FString("attack")){

		if (this->IsAttackInTurnCompleted()) {
			return false;
		}

		if (toLocation){

			LogicTile *fromTile = GetVaqueroLocation();
			
			if (toLocation == fromTile) {
				return false;
			}
		}
		return vgminstance->GetActionsPerTurn() > this->_counterActions && !this->_attackInTurnCompleted;
	}
	return vgminstance->GetActionsPerTurn() > this->_counterActions;
}

bool AVaquerosCharacter::CanDoAction()
{
	return vgminstance->GetActionsPerTurn() > this->_counterActions;
}

int AVaquerosCharacter::IncreaseCounterActions()
{
	++_counterActions;
	return _counterActions;
}

void AVaquerosCharacter::ResetCounterActions()
{
	_counterActions = 0;
}

int AVaquerosCharacter::GetCounterActions()
{
	return _counterActions;
}

bool AVaquerosCharacter::IsMoveInTurnCompleted()
{
	return _moveInTurnCompleted;
}

bool AVaquerosCharacter::IsAttackInTurnCompleted()
{
	return _attackInTurnCompleted;
}

void AVaquerosCharacter::SetMoveInTurnCompleted(bool value)
{
	_moveInTurnCompleted = value;
}

void AVaquerosCharacter::SetAttackInTurnCompleted(bool value)
{
	_attackInTurnCompleted = value;
}

void AVaquerosCharacter::SetNumMovements(int value)
{
	_numMovements = value;
}

int AVaquerosCharacter::GetNumMovements()
{
	return _numMovements;
}

bool AVaquerosCharacter::CanDoMovements(int value)
{
	return value <= _numMovements;
}

void AVaquerosCharacter::SetOldPositionMember(int xpos, int ypos, int oldrange)
{
	_oldPosition[0] = xpos;
	_oldPosition[1] = ypos;
	_oldPosition[2] = oldrange;
}

int AVaquerosCharacter::GetOldPostionMember(int index)
{
	if (index < 0 || index > 2) {
		return -1;
	}
	return _oldPosition[index];
}

LogicTile * AVaquerosCharacter::GetVaqueroLocation()
{
	Matriz* matrix = vgminstance->GetMatrix();

	FVector tilePosition = this->GetActorLocation();
	int tilePosX = matrix->getCoor(tilePosition.X);
	int tilePosY = matrix->getCoor(tilePosition.Y);

	return matrix->GetTile(tilePosX, tilePosY);
}

bool AVaquerosCharacter::makeShot(FVector _vLocation)
{
	LogicTile* pTile = vgminstance->GetMatrix()->GetTile(_vLocation);
	_objetiveToShoot = pTile->getOccupant();
	bool success = _gunManagerComponent->makeShot(GetVaqueroLocation(), pTile);

	if(success)
	{
		FVector direction = (_vLocation - GetActorLocation());
		direction.Normalize();
		float dotValue = FVector::DotProduct(direction, GetActorRotation().Vector());
		float AimAtAngle = acosf(dotValue);
		if (!FMath::IsNearlyZero(AimAtAngle, HALF_PI / 2)) {
			if (FMath::IsNearlyEqual(AimAtAngle, PI, HALF_PI / 2)) {
				_turnAround = true;
			}
			else {
				bool x = FMath::IsNearlyEqual(GetActorRightVector().X, direction.X, 0.5f);
				bool y = FMath::IsNearlyEqual(GetActorRightVector().Y, direction.Y, 0.5f);
				if (x&y) {
					_bRotateRight = true;
				}
				else {
					_bRotateLeft = true;
				}
			}
		}

		_shootAnim = true;
		
	}

	return success;
}

std::vector<LogicTile*> AVaquerosCharacter::objetivesInRange(LogicTile* tile)
{
	if (tile != nullptr)
	{
		return _gunManagerComponent->objetivesInRange(tile, this);
	}
	return _gunManagerComponent->objetivesInRange(GetVaqueroLocation(), this);
}

std::vector<LogicTile*> AVaquerosCharacter::tilesInRange(LogicTile* tile)
{
	if (tile != nullptr)
	{
		return _gunManagerComponent->tilesInRange(tile);
	}
	return _gunManagerComponent->tilesInRange(GetVaqueroLocation());
}

void AVaquerosCharacter::SetNewLocations(std::vector<LogicTile*> locations)
{
	Matriz * matrix = vgminstance->GetMatrix();
	for (LogicTile* tile : locations)
	{
		this->_newLocations.Push(matrix->locationTile(tile->getLogicalX(), tile->getLogicalY()));
	}
}

void AVaquerosCharacter::SetNewLocations(TArray<FVector> locations)
{
	for (int i = 0; i < locations.Num(); i++)
	{
		this->_newLocations.Push(locations[i]);
	}
}

void AVaquerosCharacter::MoveToTile(LogicTile * toTile)
{
	Matriz* matrix = vgminstance->GetMatrix();

	if (!matrix)
	{
		UE_LOG(LogTemp, Error, TEXT("matrix isn't valid"));
		return;
	}

	LogicTile *fromTile = this->GetVaqueroLocation();

	/*if (!toTile)
	{
		UE_LOG(LogTemp, Error, TEXT("toTile ES NULLPTR"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("toTile NO ES NULLPTR"));
	}

	UE_LOG(LogTemp, Error, TEXT("toTile->_walkable: %s"), (toTile->_walkable ? TEXT("TRUE") : TEXT("FALSE")));
	UE_LOG(LogTemp, Error, TEXT("!toTile->_occupied: %s"), (!toTile->_occupied ? TEXT("TRUE") : TEXT("FALSE")));
	UE_LOG(LogTemp, Error, TEXT("toTile != fromTile: %s"), (toTile->_walkable ? TEXT("TRUE") : TEXT("FALSE")));
	UE_LOG(LogTemp, Error, TEXT("this->_realenergy > 0: %s"), (this->_realenergy > 0 ? TEXT("TRUE") : TEXT("FALSE")));*/
	


	if (toTile && toTile->_walkable && !toTile->_occupied && this->_realenergy > 0 && toTile != fromTile)
	{

		vector<LogicTile*> path = Pathfinding::FindPath(fromTile, toTile, matrix);
		/*UE_LOG(LogTemp, Error, TEXT("NewLocations: %d"), this->_newLocations.Num());
		UE_LOG(LogTemp, Error, TEXT("NewLocations: %s"), (this->CanDoMovements(path.size()) ? TEXT("TRUE") : TEXT("FALSE")));*/
		//Avoid add other movements while character is moving
		if (this->_newLocations.Num() == 0 && this->CanDoMovements(path.size())) {

			int totalcost = 0;
			for (LogicTile* tile : path) totalcost += tile->_cost;
			bool enoughEnergy = totalcost <= this->_realenergy;

			if (!enoughEnergy) {
				UE_LOG(LogTemp, Warning, TEXT("Not enough energy to move (return)"));
				return;
			}
			
			this->SetNewLocations(path);

			//Subtract energy for each tile
			this->modifyEnergy(-totalcost);

			// Occupants
			fromTile->SetOccupant(nullptr);
			toTile->SetOccupant(this);

		}
	}
}

bool AVaquerosCharacter::IsMovementFinalized()
{

	return _newLocations.Num() == 0 && !_isMoving;;
}

void AVaquerosCharacter::ResetCoolDown()
{
	this->_gunManagerComponent->myTick();
}

void AVaquerosCharacter::GlowCharacter(bool value)
{
	TArray<USkeletalMeshComponent*> children;
	this->GetComponents<USkeletalMeshComponent>(children);
	USkeletalMeshComponent* mesh;

	UMaterialInstanceDynamic* _dynMaterial = nullptr;

	// First UStaticMeshComponent WARNING (Only this mesh)
	if (children[0]) {
		mesh = children[0];
		if (value) {
			if (_glowMaterial) {
				_dynMaterial = UMaterialInstanceDynamic::Create(_glowMaterial, this);
				if ( this == vgminstance->GetVCharacterByCharacterName("TieBilly") || this == vgminstance->GetVCharacterByCharacterName("PyroJane")) {
					mesh->SetMaterial(1, _dynMaterial);
				}
				else if ( this == vgminstance->GetVCharacterByCharacterName("FlintSkull") ) {
					mesh->SetMaterial(0, _dynMaterial);
				}
			}
		}
		else {
			if (_normalMaterial) {
				_dynMaterial = UMaterialInstanceDynamic::Create(_normalMaterial, this);
				if (Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetDayMode())
				{
					_dynMaterial->SetScalarParameterValue(FName("EXP_Emissive"), 0.f);
				}
				if (this == vgminstance->GetVCharacterByCharacterName("TieBilly") || this == vgminstance->GetVCharacterByCharacterName("PyroJane")) {
					mesh->SetMaterial(1, _dynMaterial);
				}
				else if ( this == vgminstance->GetVCharacterByCharacterName("FlintSkull") ) {
					mesh->SetMaterial(0, _dynMaterial);
				}
			}
		}
	}



	/*
	for (int i = 0; i < children.Num(); i++)
	{
		if (children[i]->GetMaterial(0)) {
			UE_LOG(LogTemp, Error, TEXT("Mesh component %s tiene material"), *children[i]->GetName());
			UE_LOG(LogTemp, Error, TEXT("Mesh Material %s "), *children[i]->GetMaterial(0)->GetName());
			UE_LOG(LogTemp, Error, TEXT("Index %d "), i);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Mesh component %s NO tiene material"), *children[i]->GetName());
		}
	}*/

}

void AVaquerosCharacter::MoveVaquerosCharToActor(LogicTile * toTile)
{
	Matriz* matrix = vgminstance->GetMatrix();

	if (!matrix)
	{
		UE_LOG(LogTemp, Error, TEXT("matrix isn't valid"));
		return;
	}

	LogicTile *fromTile = this->GetVaqueroLocation();

	if (toTile && toTile->_walkable && !toTile->_occupied && this->_realenergy > 0 && toTile != fromTile)
	{

		vector<LogicTile*> path = Pathfinding::FindPath(fromTile, toTile, matrix);

		//Avoid add other movements while character is moving
		if (this->_newLocations.Num() == 0 && this->CanDoMovements(path.size())) {

			int totalcost = 0;
			for (LogicTile* tile : path) totalcost += tile->_cost;
			bool enoughEnergy = totalcost <= this->_realenergy;

			if (!enoughEnergy) {
				UE_LOG(LogTemp, Warning, TEXT("Not enough energy to move (return)"));
				return;
			}

			this->SetNewLocations(path);
	
			//Subtract energy for each tile
			this->modifyEnergy(-totalcost);
			
			fromTile->SetOccupant(nullptr);

			//toTile->_occupied = true;
			toTile->SetOccupant(this);

			vgminstance->CompleteMoveInTurn(this);
		}

		if (this->GetCounterActions() >= vgminstance->GetActionsPerTurn()) {
			vgminstance->EndTurn(this);
		}
	}
}

void AVaquerosCharacter::Attack(LogicTile * toTile)
{
	Matriz* matrix = vgminstance->GetMatrix();

	if (!matrix)
	{
		UE_LOG(LogTemp, Error, TEXT("matrix isn't valid"));
		return;
	}

	LogicTile *fromTile = this->GetVaqueroLocation();

	if (toTile != fromTile)
	{
		if (this->makeShot(matrix->locationTile(toTile))) {
			
			vgminstance->CompleteAttackInTurn(this);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("El ataque no se puede realizar."));
		}

		if (this->GetCounterActions() >= vgminstance->GetActionsPerTurn()) {
			vgminstance->EndTurn(this);
		}
	}
}

void AVaquerosCharacter::ShowTilesInRange(bool mode)
{
	if (!vgminstance) 
		return;

	Matriz* matrix = vgminstance->GetMatrix();

	LogicTile *fromTile = nullptr;
	int numMovements = this->GetNumMovements();
	if (mode) {
		FVector fromPosition = this->GetActorLocation();
		int tileFromPosX = matrix->getCoor(fromPosition.X);
		int tileFromPosY = matrix->getCoor(fromPosition.Y);
		fromTile = matrix->GetTile(tileFromPosX, tileFromPosY);

		//Set old position member
		this->SetOldPositionMember(tileFromPosX, tileFromPosY, numMovements);
	}
	else {
		if (this->GetOldPostionMember(0) == -1 || this->GetOldPostionMember(1) == -1 || this->GetOldPostionMember(2) == -1) {
			UE_LOG(LogTemp, Warning, TEXT("_oldPosition not initialized"));
		}
		else {
			fromTile = matrix->GetTile(this->GetOldPostionMember(0), this->GetOldPostionMember(1));
			numMovements = this->GetOldPostionMember(2);
		}
	}

	if (!fromTile) {
		FVector fromPosition = this->GetActorLocation();
		int tileFromPosX = matrix->getCoor(fromPosition.X);
		int tileFromPosY = matrix->getCoor(fromPosition.Y);
		fromTile = matrix->GetTile(tileFromPosX, tileFromPosY);

		//Set old position member
		this->SetOldPositionMember(tileFromPosX, tileFromPosY, numMovements);
	}

	vector<LogicTile*> path = Pathfinding::FindNearTiles(fromTile, numMovements, matrix);
	if (!mode) path.push_back(fromTile);
	matrix->shine(path, mode, 2);
}

void AVaquerosCharacter::ShowTilesForAttack(bool mode)
{
	Matriz* matrix = vgminstance->GetMatrix();
	vector<LogicTile*> path = this->objetivesInRange(this->GetVaqueroLocation());

	matrix->shine(path, mode, 1);
}

void AVaquerosCharacter::ShowTilesInRangeForAttack(bool mode)
{
	Matriz* matrix = vgminstance->GetMatrix();
	vector<LogicTile*> path = this->_gunManagerComponent->tilesInRange(this->GetVaqueroLocation());

	matrix->shine(path, mode, 5);
}

void AVaquerosCharacter::ShowAllTiles(bool mode)
{
	ShowTilesInRange(mode);
	ShowTilesForAttack(mode);
	ShowTilesInRangeForAttack(mode);

	AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
	if (playercontroller->GetSelectedEnemy() == this) {
		playercontroller->SetIsTilesSelectedEnemyIlluminated(mode);
	}
}

void AVaquerosCharacter::SetHp(int hp)
{
	if (_hp <= _maxhp) {
		_hp = hp;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_hp can't be more than _maxhp (SetHp)"));
	}
}

void AVaquerosCharacter::AddHp(int num)
{
	if ( (_hp + num) <= _maxhp ) {
		_hp += num;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_hp can't be more than _maxhp (AddHp)"));
	}	
	if (_hp < 0)
		_hp = 0;
}

void AVaquerosCharacter::UpdateAnimationVariables(bool & stop, bool & rotateLeft, bool & rotateRight, bool &receiveDamage, bool &shootAnim, AActor* &objetiveToShoot, bool &dieAnim, bool &turnAround)
{
	stop = _bStop;
	rotateLeft = _bRotateLeft;
	rotateRight = _bRotateRight;
	receiveDamage = _receiveDamage;
	shootAnim = _shootAnim;
	objetiveToShoot = _objetiveToShoot;
	dieAnim = _dieAnim;
	turnAround = _turnAround;
}

void AVaquerosCharacter::prepareAnimationShoot(AActor * objetive)
{
	_objetiveToShoot = objetive;
	if (objetive)
		_shootAnim = true;
}

void AVaquerosCharacter::prepareReceiveDamage()
{
	if(GetHp()>0)
		_receiveDamage = true;
	else
		_dieAnim = true;
	updateHud();	
}

bool AVaquerosCharacter::CompareVaquerosPosition(const FVector & newlocation, int error)
{
	bool x = FMath::IsNearlyEqual(this->GetActorLocation().X, newlocation.X, error);
	bool y = FMath::IsNearlyEqual(this->GetActorLocation().Y, newlocation.Y, error);
	//bool z = FMath::IsNearlyEqual(this->GetActorLocation().Z, newlocation.Z, error);

	if (x && y /*&& z*/) {
		return true;
	}
	return false;
}

void AVaquerosCharacter::updateHud() {
	
	if (GetHp() >= 0) {
		FOutputDeviceNull  ar;
		const FString command = FString::Printf(TEXT("ActualizarHP %d"), GetHp());
		CallFunctionByNameWithArguments(*command, ar, this, true);
	}
}
