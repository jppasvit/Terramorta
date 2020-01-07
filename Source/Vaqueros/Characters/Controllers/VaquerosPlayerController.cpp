// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosPlayerController.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameController/VaquerosGameManager.h"
#include "Characters/Characters/VaquerosCharacter.h"
#include "MatrixLogic/Tile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Builders/TileMapBuilder.h"
#include "Utils/Pathfinding.h"
#include "Weapons/GunManager.h"
#include "VaquerosGameMode.h"


AVaquerosPlayerController::AVaquerosPlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

// Called when the game starts or when spawned
void AVaquerosPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

	//UE_LOG(LogTemp, Warning, TEXT("Mis players: %s"), *vgminstance->GetPlayers()[0]->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Players seteados: %d"), vgminstance->GetNumPlayers());
	//vgminstance->SetVCharactersOnLocation();
	gamemode = Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AVaquerosPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	this->InputComponent->BindAction("LeftClick", IE_Pressed, this, &AVaquerosPlayerController::OnClickAnyWhere);
	//this->InputComponent->BindAction("RightClick", IE_Pressed, this, &AVaquerosPlayerController::PassTurn);

}

void AVaquerosPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//VaquerosGameManager::ReleaseInstance();
	vgminstance->SetToNull();
	SetSelectedEnemy(false);
}

void AVaquerosPlayerController::OnClickAnyWhere() {

	float locationX;
	float locationY;
	//int x, y;
	this->GetMousePosition(locationX, locationY);

	FVector2D mousePosition(locationX, locationY);
	FHitResult hitResult;
	const bool bTraceComplex = false;

	if ( this->GetHitResultAtScreenPosition(mousePosition, ECC_Visibility, bTraceComplex, hitResult) == true && CheckCanClickAnyWhere() ) {

		Matriz* matrix = vgminstance->GetMatrix();
		AVaquerosCharacter* currentCharacter = Cast<AVaquerosCharacter>(vgminstance->GetCurrentCharacter());
		AActor * actorClicked = hitResult.GetActor();

		// There isn't a valid currentCharacter and the actor clicked is a AVaquerosCharacter
		if (!IsValid(currentCharacter) && actorClicked->IsA(AVaquerosCharacter::StaticClass())) {
			AVaquerosCharacter* vaquerosClicked = Cast<AVaquerosCharacter>(actorClicked);
			// vaquerosClicked isn't an enemy
			if (!vaquerosClicked->_enemy) {
				// Set actual character and in turn
				vgminstance->SetCurrentCharacterAndInTurn(vaquerosClicked);
				UE_LOG(LogTemp, Warning, TEXT("currentCharacter Seleccionado"));
				UE_LOG(LogTemp, Warning, TEXT("Character in turn: %s"), *vgminstance->GetPlayerInTurn()->GetName());
				if ( !isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy)) {
					selectedEnemy->ShowAllTiles(false);
				}
				vaquerosClicked->GlowCharacter(true);
				if (!vaquerosClicked->IsMoveInTurnCompleted()) {
					vaquerosClicked->ShowTilesInRange(true);
				}
				if (!vaquerosClicked->IsAttackInTurnCompleted()) {
					vaquerosClicked->ShowTilesForAttack(true);
					vaquerosClicked->ShowTilesInRangeForAttack(true);
				}
				gamemode->UpdateCharacter(vaquerosClicked);
				gamemode->NotifyChangesOfAttributes(vaquerosClicked);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Character is a enemy"));
				if (!isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy)) {
					selectedEnemy->ShowAllTiles(false);
				}
				if ( !vgminstance->IsCharacterPendigToDie(vaquerosClicked) ) {
					SetSelectedEnemy(vaquerosClicked);
					selectedEnemy->ShowAllTiles(true);
				}
				vgminstance->SetCurrentCharacterAndInTurn(nullptr);
			}
		} // currentCharacter is valid and the actor clicked isn't a AVaquerosCharacter
		else if ( IsValid(currentCharacter) && !actorClicked->IsA(AVaquerosCharacter::StaticClass()) && !actorClicked->GetClass()->GetName().Equals(FString("Landscape")) ) {
			// !actorClicked->GetClass()->GetName().Equals(FString("Landscape")) motivated because, for using actorClicked->IsA(ALandscape::StaticClass()) need to add Landscape module
			UE_LOG(LogTemp, Warning, TEXT("Actor clicked class: %s"), *actorClicked->GetClass()->GetName());
			LogicTile * tile = matrix->GetTile(hitResult.ImpactPoint);
			if (tile && currentCharacter->CanDoAction(FString("Move"), tile)) {
				// Tick de cooldown
				//currentCharacter->_gunManagerComponent->myTick();
				currentCharacter->ShowTilesInRange(false);
				currentCharacter->ShowTilesForAttack(false);
				currentCharacter->ShowTilesInRangeForAttack(false);
				currentCharacter->MoveVaquerosCharToActor(tile);
				gamemode->NotifyChangesOfAttributes(currentCharacter);
				UE_LOG(LogTemp, Warning, TEXT("currentCharacter Moviendose"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("currentCharacter No puede hacer la Accion 1 (Moverse)"));
			}

			if (tile && tile->getOccupant() && tile->getOccupant()->IsA(AVaquerosCharacter::StaticClass())){
				AVaquerosCharacter * character = Cast<AVaquerosCharacter>(tile->getOccupant());
				if (character->_enemy) {
					vector<LogicTile*> path = currentCharacter->objetivesInRange();
					vector<LogicTile*>::iterator it = std::find(path.begin(), path.end(), tile);
					if (tile && it != path.end() && currentCharacter->CanDoAction(FString("Attack"), tile) ){
						currentCharacter->ShowTilesForAttack(false);
						currentCharacter->ShowTilesInRangeForAttack(false);
						currentCharacter->Attack(tile);
						gamemode->NotifyChangesOfAttributes(currentCharacter);
						UE_LOG(LogTemp, Warning, TEXT("currentCharacter Ataque 2.1"));
						//vaquerosClicked->Destroy();
						//currentCharacter->_gunManagerComponent->myTick();
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("currentCharacter No puede hacer la Accion 2.2 (Atacar)"));
						currentCharacter->ShowAllTiles(false);
						currentCharacter->GlowCharacter(false);
						if ( !isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy) ) {
							selectedEnemy->ShowAllTiles(false);
						}
						if (!vgminstance->IsCharacterPendigToDie(character)) {
							SetSelectedEnemy(character);
							selectedEnemy->ShowAllTiles(true);
						}
						vgminstance->SetCurrentCharacterAndInTurn(nullptr);
					}
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Occupant isn't a AVaquerosCharacter"));
			}

		} // currentCharacter is valid and the actor clicked is a AVaquerosCharacter
		else if (IsValid(currentCharacter) && actorClicked->IsA(AVaquerosCharacter::StaticClass())) {

			AVaquerosCharacter* vaquerosClicked = Cast<AVaquerosCharacter>(actorClicked);

			// vaquerosClicked is an enemy
			if (vaquerosClicked->_enemy) {
				LogicTile * tile = matrix->GetTile(hitResult.ImpactPoint);
				vector<LogicTile*> path = currentCharacter->objetivesInRange();
				vector<LogicTile*>::iterator it = std::find(path.begin(), path.end(), tile);
				if(tile && it != path.end() && currentCharacter->CanDoAction(FString("Attack"), tile) && currentCharacter->_gunManagerComponent->canShoot(currentCharacter->GetVaqueroLocation(), tile)){
					currentCharacter->ShowTilesForAttack(false);
					currentCharacter->ShowTilesInRangeForAttack(false);
					currentCharacter->Attack(tile);
					gamemode->NotifyChangesOfAttributes(currentCharacter);
					UE_LOG(LogTemp, Warning, TEXT("currentCharacter Ataque 2.2"));
					//vaquerosClicked->Destroy();
					//currentCharacter->_gunManagerComponent->myTick();
					if (!isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy) ) {
						selectedEnemy->ShowAllTiles(false);
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("currentCharacter No puede hacer la Accion 2.2 (Atacar)"));
					currentCharacter->ShowAllTiles(false);
					currentCharacter->GlowCharacter(false);
					if ( !isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy) ) {
						selectedEnemy->ShowAllTiles(false);
					}
					if (!vgminstance->IsCharacterPendigToDie(vaquerosClicked)) {
						SetSelectedEnemy(vaquerosClicked);
						selectedEnemy->ShowAllTiles(true);
					}

					vgminstance->SetCurrentCharacterAndInTurn(nullptr);
				}
			}
			else {
				// Set current character
				if (vgminstance->CanPassTurnToPlayer(vaquerosClicked)) {

					if ( !isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy)) {
						selectedEnemy->ShowAllTiles(false);
					}

					currentCharacter->ShowTilesInRange(false);
					currentCharacter->ShowTilesForAttack(false);
					currentCharacter->ShowTilesInRangeForAttack(false);
					currentCharacter->GlowCharacter(false);
					if (!isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy) ) {
						selectedEnemy->ShowAllTiles(false);
					}
					gamemode->NotifyChangesOfAttributes(currentCharacter);
					// Set current character and in turn
					vgminstance->SetCurrentCharacterAndInTurn(vaquerosClicked);
					UE_LOG(LogTemp, Warning, TEXT("currentCharacter Seleccionado 2"));
					UE_LOG(LogTemp, Warning, TEXT("Character in turn: %s"), *vgminstance->GetPlayerInTurn()->GetName());
					if (!vaquerosClicked->IsMoveInTurnCompleted()) {
						vaquerosClicked->ShowTilesInRange(true);
					}
					if (!vaquerosClicked->IsAttackInTurnCompleted()) {
						vaquerosClicked->ShowTilesForAttack(true);
						vaquerosClicked->ShowTilesInRangeForAttack(true);
					}

					gamemode->UpdateCharacter(vaquerosClicked);

					if (!vgminstance->IsCharacterActionsCompleted(vaquerosClicked)) {
						vaquerosClicked->GlowCharacter(true);
					}
					gamemode->NotifyChangesOfAttributes(vaquerosClicked);
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("El turno no se puede pasar."));
				}
			}

		}
		// Check global player turn
		vgminstance->CheckGlobalPlayersTurn();
		//vgminstance->LogCharacters();

		if (vgminstance->IsPlayersTurnEnded()) {
			//vgminstance->StartTurnPlayers();
			//UE_LOG(LogTemp, Warning, TEXT("Todos los players han acabado su turno. Se resetean sus valores de turno"));
			currentCharacter->GlowCharacter(false);
			currentCharacter->ShowTilesInRangeForAttack(false);
			if ( !isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy) ) {
				selectedEnemy->ShowAllTiles(false);
			}
			gamemode->ChangeTurn(true);
			UE_LOG(LogTemp, Warning, TEXT("Todos los players han acabado su turno, empieza el de los enemigos"));
		}
	}
}


void AVaquerosPlayerController::PassTurn()
{
	AVaquerosCharacter * vPlayerInTurn = vgminstance->GetPlayerInTurn();
	AVaquerosCharacter * vActualCharacter = Cast<AVaquerosCharacter>(vgminstance->GetCurrentCharacter());

	if( CheckCanClickAnyWhere() ){
		if (vPlayerInTurn != vActualCharacter || vPlayerInTurn == nullptr || vActualCharacter == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("ActualCharacter no es igual al Player en turno. No se pasa turno"));
		}
		else {
			vgminstance->EndTurn(vActualCharacter);
			vActualCharacter->ShowTilesInRange(false);
			vActualCharacter->ShowTilesForAttack(false);
			vActualCharacter->ShowTilesInRangeForAttack(false);
			vActualCharacter->GlowCharacter(false);
			UE_LOG(LogTemp, Warning, TEXT("ActualCharacter pasa el turno"));
		}

		if (vgminstance->IsPlayersTurnEnded()) {
			//vgminstance->StartTurnPlayers();
			//UE_LOG(LogTemp, Warning, TEXT("Todos los players han acabado su turno. Se resetean sus valores de turno"));
			vActualCharacter->GlowCharacter(false);
			vActualCharacter->ShowTilesInRangeForAttack(false);
			gamemode->ChangeTurn(true);
			UE_LOG(LogTemp, Warning, TEXT("Todos los players han acabado su turno, empieza el de los enemigos"));
		}

		if (!isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy) ) {
			selectedEnemy->ShowAllTiles(false);
		}
	}
}

bool AVaquerosPlayerController::CheckCanClickAnyWhere()
{
	for (int i = 0; i < vgminstance->GetNumPlayers(); i++)
	{
		if (!vgminstance->GetPlayer(i)->IsMovementFinalized()) {
			return false;
		}
	}

	for (int i = 0; i < vgminstance->GetNumEnemies(); i++)
	{
		if (!vgminstance->GetEnemy(i)->IsMovementFinalized()) {
			return false;
		}

		if ( vgminstance->IsEnemyInTurn(vgminstance->GetEnemy(i)) ) {
			return false;
		}
	}

	return true;
}

void AVaquerosPlayerController::SetSelectedEnemy(AVaquerosCharacter * enemy)
{
	if (enemy == nullptr || enemy->_enemy ) {
		selectedEnemy = enemy;
		SetIsSelectedEnemyDestroyed(false);
	}

	if (enemy == vgminstance->GetCharacterPendigToDie()) {
		selectedEnemy = nullptr;
	}
}

AVaquerosCharacter * AVaquerosPlayerController::GetSelectedEnemy()
{
	return selectedEnemy;
}

void AVaquerosPlayerController::SetIsTilesSelectedEnemyIlluminated(bool var)
{
	if ( !isSelectedEnemyDestroyed && selectedEnemy && IsValid(selectedEnemy)) {
		isTilesSelectedEnemyIlluminated = var;
	}
}

bool AVaquerosPlayerController::IsTilesSelectedEnemyIlluminated()
{
	return isTilesSelectedEnemyIlluminated;
}

bool AVaquerosPlayerController::IsSelectedEnemyDestroyed()
{
	return isSelectedEnemyDestroyed;
}

void AVaquerosPlayerController::SetIsSelectedEnemyDestroyed(bool var)
{
	isSelectedEnemyDestroyed = var;
}

