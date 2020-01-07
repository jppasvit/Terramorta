// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController/VaquerosGameManager.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Characters/Characters/VaquerosCharacter.h"
#include "MatrixLogic/Matriz.h"
#include "VaquerosGameInstance.h"
#include "Utils/Pathfinding.h"


VaquerosGameManager* VaquerosGameManager::instance = 0;

VaquerosGameManager::VaquerosGameManager() {
	_actualCharacter = nullptr;
}

VaquerosGameManager::~VaquerosGameManager()
{
}

VaquerosGameManager* VaquerosGameManager::GetInstance()
{
	// TO-DO Asegurarse que se establece correctamente el maximo numero de jugadores
	if (instance == nullptr) {
		instance = new VaquerosGameManager();
		if (instance->_maxNumPlayers <= 0 ) {
			instance->_maxNumPlayers = 8;
		}
		instance->_numplayers = 0;
		instance->_players = new AVaquerosCharacter*[instance->_maxNumPlayers];

		if (instance->_maxNumEnemies <= 0) {
			instance->_maxNumEnemies = 15;
		}
		instance->_numenemies = 0;
		instance->_enemies = new AVaquerosCharacter*[instance->_maxNumEnemies];

		instance->_indexOfPlayerInTurn = -1;
	}

	return instance;
}

void VaquerosGameManager::ReleaseInstance()
{
	delete instance;
	instance = NULL;
}

void VaquerosGameManager::SetToNull()
{
	
	StartTurnPlayers();
	StartTurnEnemies();

	for (int i = 0; i < GetNumPlayers(); i++)
	{
		GetPlayers()[i] = nullptr;
	}

	for (int i = 0; i < GetNumEnemies(); i++)
	{
		GetEnemies()[i] = nullptr;
	}

	_numplayers = 0;
	_numenemies = 0;
	//SetMatrix(nullptr);
}

void VaquerosGameManager::SetCurrentCharacter(ACharacter* actualCharacter)
{	
	_actualCharacter = actualCharacter;
}

ACharacter* VaquerosGameManager::GetCurrentCharacter()
{
	return _actualCharacter;
}

Matriz * VaquerosGameManager::GetMatrix()
{
	return _matrix;
}

void VaquerosGameManager::SetMatrix(Matriz* matrix)
{
	_matrix = matrix;
}

void VaquerosGameManager::SetVCharactersInGame(TArray<AActor*> characters)
{
	Matriz* matrix = GetMatrix();
	
	AVaquerosCharacter * character = nullptr;
	FVector toPosition;

	for (int i = 0; i < characters.Num(); i++)
	{
		character = Cast<AVaquerosCharacter>(characters[i]);
		if (character && character->_putOnInitialPosition)
		{
			if (!character->_enemy) {
				SetPlayer(character);
			}
			else {
				SetEnemy(character);
			}
			toPosition = matrix->locationTile(character->_xPosition, character->_yPosition);
			character->SetActorLocation(FVector(toPosition.X, toPosition.Y, character->GetActorLocation().Z));
			matrix->GetTile(matrix->getCoor(toPosition.X), matrix->getCoor(toPosition.Y))->SetOccupant(character);
		}
	}
}

AVaquerosCharacter * VaquerosGameManager::GetVCharacterByCharacterName(FString name)
{
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		if (GetPlayer(i) && GetPlayer(i)->GetNameCharacter().Equals(name)) {
			return GetPlayer(i);
		}
	}

	for (int i = 0; i < GetNumEnemies(); i++)
	{
		if (GetEnemy(i) && GetEnemy(i)->GetNameCharacter().Equals(name)) {
			return GetEnemy(i);
		}
	}

	return nullptr;
}

AVaquerosCharacter** VaquerosGameManager::GetPlayers()
{
	return _players;
}

void VaquerosGameManager::SetPlayer(AVaquerosCharacter * player)
{

	if ((_numplayers + 1) > _maxNumPlayers) {
		UE_LOG(LogTemp, Error, TEXT("Numero de jugadores excedido, cambie el numero maximo de jugadores permitidos"));
		return;
	}
	GetPlayers()[_numplayers] = player;
	_numplayers++;

}

void VaquerosGameManager::SetPlayers(AVaquerosCharacter** players)
{
	_players = players;
}


AVaquerosCharacter * VaquerosGameManager::GetPlayer(int pos)
{
	
	if (GetNumPlayers() > 0 && pos >= 0 && pos <= GetNumPlayers()) {
		return GetPlayers()[pos];
	}
	return nullptr;
}

int VaquerosGameManager::GetNumPlayers()
{
	return _numplayers;
}

int VaquerosGameManager::GetIndexPlayer(AVaquerosCharacter * player)
{
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		if (GetPlayer(i) == player) {
			return i;
		}
	}
	return -1;
}

void VaquerosGameManager::SetNumMaxPlayers(int maxNumPlayers)
{
	_maxNumPlayers = maxNumPlayers;
	delete [] _players;
	_players = new AVaquerosCharacter*[maxNumPlayers];
	_numplayers = 0;
}

AVaquerosCharacter * VaquerosGameManager::GetPlayerInTurn()
{
	
	int index = GetIndexOfPlayerInTurn();
	if (index < 0 || 
		index >= GetNumPlayers() || 
		index == -1 ) 
	{
		UE_LOG(LogTemp, Error, TEXT("GetPlayerInTurn: No puede devolver un jugador (GetIndexOfPlayerInTurn o GetNumPlayers)"));
		return nullptr;
	}
	return _players[_indexOfPlayerInTurn];
}

int VaquerosGameManager::SetPlayerInTurn(AVaquerosCharacter * player)
{
	
	int index = GetIndexPlayer(player);
	if (index == -1) {
		UE_LOG(LogTemp, Error, TEXT("No se encuentra el jugador"));
	}
	else {
		SetIndexOfPlayerInTurn(index);
		//StartTurn(player);
	}
	return index;
}

int VaquerosGameManager::GetIndexOfPlayerInTurn()
{
	return _indexOfPlayerInTurn;
}

void VaquerosGameManager::SetIndexOfPlayerInTurn(int index)
{
	_indexOfPlayerInTurn = index;
}

void VaquerosGameManager::SetCurrentCharacterAndInTurn(AVaquerosCharacter * player)
{
	
	SetCurrentCharacter(player);
	SetPlayerInTurn(player);

}

void VaquerosGameManager::StartTurn(AVaquerosCharacter * player)
{
	player->SetTurnCompleted(false);
	player->SetMoveInTurnCompleted(false);
	player->SetAttackInTurnCompleted(false);
	player->ResetCounterActions();
	player->ResetCoolDown();
	
}

void VaquerosGameManager::StartTurnPlayers()
{
	
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		StartTurn(GetPlayer(i));
	}
	SetCurrentCharacter(nullptr);
	SetIndexOfPlayerInTurn(-1);
}

void VaquerosGameManager::EndTurn(AVaquerosCharacter * player)
{
	player->SetTurnCompleted(true);
	player->SetMoveInTurnCompleted(true);
	player->SetAttackInTurnCompleted(true);

	for (int i = player->GetCounterActions(); i < _actionsPerTurn; i++)
	{
		player->IncreaseCounterActions();
	}
}

void VaquerosGameManager::EndMovementTurn(AVaquerosCharacter * player)
{
	player->SetMoveInTurnCompleted(true);
}

void VaquerosGameManager::EndAttackTurn(AVaquerosCharacter * player)
{
	player->SetAttackInTurnCompleted(true);
}

void VaquerosGameManager::EndTurnPlayers()
{
	
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		EndTurn(GetPlayer(i));
	}
}

bool VaquerosGameManager::IsPlayersTurnEnded()
{
	
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		if (!GetPlayer(i)->IsTurnCompleted()) {
			return false;
		}
	}
	return true;
}

bool VaquerosGameManager::CanPassTurnToPlayer(AVaquerosCharacter * player)
{
	
	int indexnewplayer = GetIndexPlayer(player);
	if ( indexnewplayer == -1) { 
		UE_LOG(LogTemp, Warning, TEXT("No se encuentra el jugador (CanPassTurnToPlayer)"));
		return false;
	}
	else {
		int indexturnplayer = GetIndexOfPlayerInTurn();
		if (indexturnplayer == -1) { 
			UE_LOG(LogTemp, Warning, TEXT("No se encuentra jugador en turno (CanPassTurnToPlayer)"));
			return true;
		}
		else if (indexnewplayer == indexturnplayer) {
			UE_LOG(LogTemp, Warning, TEXT("Juagador y jugador en turno son el mismo (CanPassTurnToPlayer)"));
			return false;
		}
		else {
			/*AVaquerosCharacter * playerInTurn = GetPlayerInTurn();
			if (!playerInTurn->IsTurnCompleted() && !playerInTurn->IsAttackInTurnCompleted() && !playerInTurn->IsMoveInTurnCompleted() && !player->IsTurnCompleted()) {
				UE_LOG(LogTemp, Warning, TEXT("Se puede cambiar de jugador, el turno individual no se ha comenzado (CanPassTurnToPlayer)"));
				return true;
			}
			else if (!playerInTurn->IsTurnCompleted() && (playerInTurn->IsAttackInTurnCompleted() || playerInTurn->IsMoveInTurnCompleted() ) ) {
				UE_LOG(LogTemp, Warning, TEXT("Juagador en turno, no ha completado su turno y ya lo ha comenzado (CanPassTurnToPlayer)"));
				return false;
			}
			else if (player->IsTurnCompleted()) {
				UE_LOG(LogTemp, Warning, TEXT("Juagador ha completado ya su turno (CanPassTurnToPlayer)"));
				return false;
			}*/
			
		}
	}
	return true;
}

int VaquerosGameManager::GetActionsPerTurn()
{
	return _actionsPerTurn;
}

void VaquerosGameManager::SetActionsPerTurn(int value)
{
	_actionsPerTurn = value;
}

/*
Updates enemy in turn, one by one. 
Returns true if new enemy in turn has been updated successfully, returns false otherwise
*/
bool VaquerosGameManager::NextEnemyTurn()
{
	
	int newIndexOfEnemyInTurn = GetIndexOfEnemyInTurn();
	int numEnemies = GetNumEnemies();

	if (newIndexOfEnemyInTurn >= -1 && newIndexOfEnemyInTurn < numEnemies) {
		if (newIndexOfEnemyInTurn == -1) {
			newIndexOfEnemyInTurn = 0;
		}
		else if (newIndexOfEnemyInTurn == (numEnemies - 1)) {
			// Enemy turn finished
			SetIndexOfEnemyInTurn(-1);
			// Se puede cambiar a IsEnemiesTurnEnded
			//_gamemode->ChangeTurn(false);
			return false;
		}
		else {
			// Next enemy
			newIndexOfEnemyInTurn++;
		}

		AVaquerosCharacter * enemy = GetEnemy(newIndexOfEnemyInTurn);
		if (enemy) {
			SetEnemyInTurn(enemy);
			return true;
		}
		UE_LOG(LogTemp, Error, TEXT("No se encuentra el enemigo en el array (NextEnemyTurn)"));
	}

	return false;
}

bool VaquerosGameManager::IsEnemyInTurn(AVaquerosCharacter * enemy)
{
	return GetEnemyInTurn() == enemy;
}

bool VaquerosGameManager::IsEnemiesTurnEnded()
{
	
	for (int i = 0; i < GetNumEnemies(); i++)
	{
		if (!GetEnemy(i)->IsTurnCompleted()) {
			return false;
		}
	}
	return true;
}

void VaquerosGameManager::EndTurnEnemies()
{
	
	for (int i = 0; i < GetNumEnemies(); i++)
	{
		EndTurn(GetEnemy(i));
	}
}

void VaquerosGameManager::StartTurnEnemies()
{
	
	for (int i = 0; i < GetNumEnemies(); i++)
	{
		StartTurn(GetEnemy(i));
	}
	SetIndexOfEnemyInTurn(-1);
}

bool VaquerosGameManager::EnemyTurnEnded(AVaquerosCharacter * enemy)
{
	//Revisar esto porque puede petar	
	if (IsEnemyInTurn(enemy) && enemy->IsMoveInTurnCompleted() && enemy->IsAttackInTurnCompleted()) {
		//enemy->SetTurnCompleted(enemy);
		if(enemy) enemy->SetTurnCompleted(true);
		if (!NextEnemyTurn()) {
			StartTurnPlayers();
			StartTurnEnemies();
			UE_LOG(LogTemp, Warning, TEXT("StartTurnPlayers()"));
			UE_LOG(LogTemp, Warning, TEXT("StartTurnEnemies();"));
			return true;
		}
	}
	return false;
}

bool VaquerosGameManager::FinishEnemyTurn(AVaquerosCharacter * enemy)
{
	
	
	if (enemy) {
		enemy->SetAttackInTurnCompleted(true);
		enemy->SetMoveInTurnCompleted(true);
		enemy->SetTurnCompleted(true);
		if (!NextEnemyTurn()) {
			StartTurnPlayers();
			StartTurnEnemies();
			UE_LOG(LogTemp, Warning, TEXT("StartTurnPlayers()"));
			UE_LOG(LogTemp, Warning, TEXT("StartTurnEnemies();"));
			return true;
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FinishEnemyTurn: enemy isn't valid "));
	}

	return false;
}

bool VaquerosGameManager::EnemyCheckTurn(AVaquerosCharacter * enemy)
{
	

	if (IsPlayersTurnEnded()) {
		if (GetIndexOfEnemyInTurn() == -1) {
			NextEnemyTurn();
			return true;
		}
		else if (IsEnemyInTurn(enemy)) {

			return true;
		}

	}
	return false;
}

bool VaquerosGameManager::IsCharacterTurnCompleted(AVaquerosCharacter * character)
{
	return character->IsMoveInTurnCompleted() && character->IsAttackInTurnCompleted() && character->IsTurnCompleted();
}

bool VaquerosGameManager::IsCharacterActionsCompleted(AVaquerosCharacter * character)
{
	return character->IsMoveInTurnCompleted() && character->IsAttackInTurnCompleted();
}

void VaquerosGameManager::CheckGlobalPlayersTurn()
{
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		AVaquerosCharacter * character = GetPlayer(i);
		if (CheckPlayerTurn(character)) {
			//LogCharacter(character);
			EndTurn(character);
		}
	}
}

bool VaquerosGameManager::CheckPlayerTurn(AVaquerosCharacter * character)
{
	if (character) {
		if (character->IsMovementFinalized() && (IsCharacterActionsCompleted(character) || CheckVaqueroTurn(character))) {
			//LogCharacter(character);
			return true;
		}
	}
	return false;
}

bool VaquerosGameManager::CheckVaqueroTurn(AVaquerosCharacter * character)
{
	Matriz* matrix = GetMatrix();
	if (character) {
		if (!character->IsMoveInTurnCompleted()) {
			vector<LogicTile*> path = Pathfinding::FindNearTiles(character->GetVaqueroLocation(), character->GetNumMovements(), matrix);
			if (path.size() > 0) {
				return false;
			}
		}

		if (character->IsMoveInTurnCompleted() && !character->IsAttackInTurnCompleted()) {
			vector<LogicTile*> path = character->objetivesInRange(character->GetVaqueroLocation());
			if (path.size() > 0) {
				return false;
			}
		}
	}

	return true;
}

void VaquerosGameManager::CompleteMoveInTurn(AVaquerosCharacter * player)
{
	player->SetMoveInTurnCompleted(true);
	player->IncreaseCounterActions();
}

void VaquerosGameManager::CompleteAttackInTurn(AVaquerosCharacter * player)
{
	player->SetAttackInTurnCompleted(true);
	player->IncreaseCounterActions();
}

void VaquerosGameManager::DestroyVaquerosCharacter(AVaquerosCharacter* vcharacter, bool anim)
{
	
	if(!vcharacter->_enemy){
		int indexToDelete = GetIndexPlayer(vcharacter);
		int numPlayers = GetNumPlayers();

		if(indexToDelete > -1){

			int indexOfPlayerInTurn = GetIndexOfPlayerInTurn();

			if (indexToDelete == indexOfPlayerInTurn) {
				SetIndexOfPlayerInTurn(-1);
				_actualCharacter = nullptr;
			}
	
			for (int i = indexToDelete; i < numPlayers - 1; i++)
			{
				_players[i] = _players[i + 1];
			}

			_numplayers--;
			vcharacter->GetVaqueroLocation()->SetOccupant(nullptr);
			if (!anim)
			{
				vcharacter->Destroy();
			}

			if (indexOfPlayerInTurn > indexToDelete && indexOfPlayerInTurn < numPlayers) {
				SetCurrentCharacterAndInTurn(GetPlayer(indexOfPlayerInTurn - 1));
			}
		
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("DestroyPlayer: Error el juagador no se encuentra en el array"));
		}
	}
	else {
		int indexToDelete = GetIndexEnemy(vcharacter);
		int numEnemies = GetNumEnemies();

		if (indexToDelete > -1) {
			int indexOfEnemyInTurn = GetIndexOfEnemyInTurn();

			if (indexToDelete == indexOfEnemyInTurn && indexOfEnemyInTurn == (numEnemies - 1) ) {
				NextEnemyTurn();
			}

			for (int i = indexToDelete; i < numEnemies - 1; i++)
			{
				_enemies[i] = _enemies[i + 1];
			}

			_numenemies--;
			vcharacter->GetVaqueroLocation()->SetOccupant(nullptr);
			SetCharacterPendingToDie(vcharacter);
			if(!anim)
			{
				vcharacter->Destroy();
			}
			

			if (indexOfEnemyInTurn > indexToDelete && indexOfEnemyInTurn < numEnemies) {
				SetEnemyInTurn(GetEnemy(indexOfEnemyInTurn - 1));
			}

		}
		else {
			UE_LOG(LogTemp, Error, TEXT("DestroyPlayer: Error el enemigo no se encuentra en el array"));
		}
	}

	AVaquerosCharacter * currentCharacter = Cast<AVaquerosCharacter>(GetCurrentCharacter());
	if (currentCharacter){
		if (!currentCharacter->IsMoveInTurnCompleted()) {
			vector<LogicTile*> path = Pathfinding::FindNearTiles(currentCharacter->GetVaqueroLocation(), currentCharacter->GetNumMovements(), GetMatrix());
			GetMatrix()->shine(path, true, 2);
		}
	}

	UVaquerosGameInstance * gameinstance = Cast<UVaquerosGameInstance>(vcharacter->GetGameInstance());
	if (HavePlayersWon()) {
		gameinstance->notifyEndGame(true);
	}
	else if (HaveEnemiesWon()) {
		gameinstance->notifyEndGame(false);
	}
}

bool VaquerosGameManager::HavePlayersWon()
{
	return GetNumEnemies() <= 0;
}

bool VaquerosGameManager::HaveEnemiesWon()
{
	return GetNumPlayers() <= 0;
}

void VaquerosGameManager::SetCharacterPendingToDie(AVaquerosCharacter * character)
{
	characterPendingToDie = character;
}

AVaquerosCharacter * VaquerosGameManager::GetCharacterPendigToDie()
{
	return characterPendingToDie;
}

bool VaquerosGameManager::IsCharacterPendigToDie(AVaquerosCharacter * character)
{
	if (characterPendingToDie && character && character == characterPendingToDie) {
		return true;
	}
	return false;
}

void VaquerosGameManager::LogCharacter(AVaquerosCharacter * character)
{
	if (character) {
		UE_LOG(LogTemp, Warning, TEXT("- - - - - - -"));
		UE_LOG(LogTemp, Warning, TEXT(" "));
		UE_LOG(LogTemp, Warning, TEXT("Character Name: %s"), *character->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Character Move turn completed: %d"), character->IsMoveInTurnCompleted());
		UE_LOG(LogTemp, Warning, TEXT("Character Attack turn completed: %d"), character->IsAttackInTurnCompleted());
		UE_LOG(LogTemp, Warning, TEXT("Character Turn completed: %d"), character->IsTurnCompleted());
		UE_LOG(LogTemp, Warning, TEXT("Character counter actions: %d"), character->GetCounterActions());
		UE_LOG(LogTemp, Warning, TEXT(" "));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("character doesn't exist"));
	}
	
}

void VaquerosGameManager::LogCharacters()
{
	for (int i = 0; i < GetNumPlayers(); i++)
	{
		LogCharacter(GetPlayer(i));
	}

	/*
	for (int i = 0; i < GetNumEnemies(); i++)
	{
		LogCharacter(GetEnemy(i));
	}
	*/
}


// Enemies

void VaquerosGameManager::SetNumMaxEnemies(int maxNumEnemies)
{
	
	_maxNumEnemies = maxNumEnemies;
	delete[] _enemies;
	_enemies = new AVaquerosCharacter*[maxNumEnemies];
	_numenemies = 0;
}

void VaquerosGameManager::SetEnemy(AVaquerosCharacter * enemy)
{
	
	if ((_numenemies + 1) > _maxNumEnemies) {
		UE_LOG(LogTemp, Error, TEXT("Numero de enemigos excedido, cambie el numero maximo de enemigos permitidos"));
		return;
	}
	GetEnemies()[_numenemies] = enemy;
	_numenemies++;
}

AVaquerosCharacter * VaquerosGameManager::GetEnemy(int pos)
{
	
	if (GetNumEnemies() > 0 && pos >= 0 && pos <= GetNumEnemies()) {
		return GetEnemies()[pos];
	}
	return nullptr;
}

AVaquerosCharacter ** VaquerosGameManager::GetEnemies()
{
	return _enemies;
}

int VaquerosGameManager::GetNumEnemies()
{
	return _numenemies;
}

int VaquerosGameManager::GetIndexEnemy(AVaquerosCharacter * enemy)
{
	
	for (int i = 0; i < GetNumEnemies(); i++)
	{
		if (GetEnemy(i) == enemy) {
			return i;
		}
	}
	return -1;
}

AVaquerosCharacter * VaquerosGameManager::GetEnemyInTurn()
{
	
	int index = GetIndexOfEnemyInTurn();
	if (index < 0 ||
		index >= GetNumEnemies() ||
		index == -1)
	{
		UE_LOG(LogTemp, Error, TEXT("GetEnemyInTurn: No puede devolver un enemigo (GetIndexOfEnemyInTurn o GetNumEnemies)"));
		return nullptr;
	}
	return _enemies[_indexOfEnemyInTurn];
}

int VaquerosGameManager::SetEnemyInTurn(AVaquerosCharacter * player)
{
	
	int index = GetIndexEnemy(player);
	if (index == -1) {
		UE_LOG(LogTemp, Error, TEXT("No se encuentra el enemigo"));
	}
	else {
		SetIndexOfEnemyInTurn(index);
		//StartTurn(player);
	}
	return index;
}

int VaquerosGameManager::GetIndexOfEnemyInTurn()
{
	return _indexOfEnemyInTurn;
}

void VaquerosGameManager::SetIndexOfEnemyInTurn(int index)
{
	_indexOfEnemyInTurn = index;
}



