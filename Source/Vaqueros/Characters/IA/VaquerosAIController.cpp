// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosAIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Utils/Pathfinding.h"
#include "GameController/VaquerosGameManager.h"
#include "VaquerosGameMode.h"


void AVaquerosAIController::BeginPlay()
{
	Super::BeginPlay();
	//// Asegurarse de que existe un target point al menos
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), _tilesPoints);
	//for (size_t i = 0; i < _tilesPoints.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("AVaquerosAIController::BeginPlay(): %s ") , *_tilesPoints[i]->GetFullName());
	//}

	

	//GoToRandomTilePoint();


}

bool AVaquerosAIController::CharacterMovementFinalized()
{
	AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
	return pawn->IsMovementFinalized();
}

bool AVaquerosAIController::UpdateTarget()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	AVaquerosCharacter* vaquerosPawn = Cast<AVaquerosCharacter>(GetPawn());
	if (!matrix)
	{
		return false;
	}

	AVaquerosCharacter* target = getClosestPlayer(vaquerosPawn, matrix);

	pBlackboardComponent->SetValueAsObject("Target", target);

	return true;
}

bool AVaquerosAIController::isTargetValid()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
	AVaquerosCharacter* target = (AVaquerosCharacter*) pBlackboardComponent->GetValueAsObject("Target");
	if(target)
	{
		return VaquerosGameManager::GetInstance()->GetIndexPlayer(target) >= 0;
	}
	return false;
}


bool AVaquerosAIController::GetRandomTilePoint()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();

	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	AVaquerosCharacter* vaquerosPawn = Cast<AVaquerosCharacter>(GetPawn());
	if(!matrix)
	{
		return false;
	}
	vector<LogicTile*> reachableTiles = Pathfinding::FindNearTiles(vaquerosPawn->GetVaqueroLocation(), vaquerosPawn->GetNumMovements(),
		matrix);

	if(reachableTiles.size()>0)
	{
		int32 index = FMath::RandRange(0, reachableTiles.size() - 1);
		//return Cast<ATargetPoint>(reachableTiles[index]);
		FVector ret = matrix->locationTile(reachableTiles[index]);

		/*AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
		LogicTile* currentLocation = pawn->GetVaqueroLocation();
		LogicTile* finalLocation = reachableTiles[index];


		_tilesPath = Pathfinding::FindPath(currentLocation, finalLocation, VaquerosGameManager::GetInstance()->GetMatrix());*/

		if (reachableTiles[index] && reachableTiles[index]->_walkable && !reachableTiles[index]->_occupied) {
			pBlackboardComponent->SetValueAsVector("TileToMove", ret);
			return true;
		}
	}
	return false;
}

bool AVaquerosAIController::GetBestTilePoint()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();

	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	AVaquerosCharacter* vaquerosPawn = Cast<AVaquerosCharacter>(GetPawn());
	if (!matrix)
	{
		return false;
	}
	LogicTile* tileResult = Pathfinding::FindPositionWithMoreEnemies(vaquerosPawn, matrix);

	if (tileResult)
	{
		FVector ret = matrix->locationTile(tileResult);

		pBlackboardComponent->SetValueAsVector("TileToMove", ret);
		return true;
	}
	return false;
}

bool AVaquerosAIController::GetClosestEnemyTilePoint()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();

	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	AVaquerosCharacter* vaquerosPawn = Cast<AVaquerosCharacter>(GetPawn());
	if (!matrix)
	{
		return false;
	}

	AVaquerosCharacter* player = getClosestPlayer(vaquerosPawn, matrix);
	
	vector<LogicTile*> positions = Pathfinding::FindClosestPath(vaquerosPawn->GetVaqueroLocation(),
		player->GetVaqueroLocation(), vaquerosPawn->GetNumMovements(), matrix);

	if (positions.size() > 0)
	{
		FVector ret = matrix->locationTile(positions.back());

		pBlackboardComponent->SetValueAsVector("TileToMove", ret);
		return true;
	}
	return false;
}

bool AVaquerosAIController::GetClosestShotPoint()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();

	Matriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	AVaquerosCharacter* vaquerosPawn = Cast<AVaquerosCharacter>(GetPawn());
	if (!matrix)
	{
		return false;
	}

	AVaquerosCharacter* player = (AVaquerosCharacter*) pBlackboardComponent->GetValueAsObject("Target");
	if(!player)
	{
		player = getClosestPlayer(vaquerosPawn, matrix);
	}
	
	LogicTile* playerPos = player->GetVaqueroLocation();

	vector<LogicTile*> shotPoints = vaquerosPawn->tilesInRange(playerPos);
	vector<LogicTile*> movePoints = Pathfinding::FindNearTiles(vaquerosPawn->GetVaqueroLocation(), vaquerosPawn->GetNumMovements(), matrix);
	vector<LogicTile*> newLocations;

	for(LogicTile* tile : shotPoints)
	{
		if(!tile->_occupied && std::find(movePoints.begin(), movePoints.end(), tile) != movePoints.end())
		{
			newLocations.push_back(tile);
		}
	}

	LogicTile* returnTile = nullptr;
	if (newLocations.size() > 0)
	{
		int distance = -1;
		for(LogicTile* newLocation : newLocations)
		{
			int aux = Pathfinding::FindPath(vaquerosPawn->GetVaqueroLocation(), newLocation, matrix).size();
			if(aux < distance || distance == -1)
			{
				returnTile = newLocation;
				distance = aux;
			}
		}
	}
	else
	{
		vector<LogicTile*> auxVector = Pathfinding::FindClosestPath(vaquerosPawn->GetVaqueroLocation(), playerPos, vaquerosPawn->GetNumMovements(), matrix);

		if(auxVector.size() > 0)
		{
			returnTile = auxVector.back();

		}
	}

	if(returnTile)
	{
		pBlackboardComponent->SetValueAsVector("TileToMove", matrix->locationTile(returnTile));
		return true;
	}

	return false;
}


void AVaquerosAIController::GoToRandomTilePoint()
{
	
	//FVector point = GetRandomTilePoint();
	//UE_LOG(LogTemp, Error, TEXT("AVaquerosAIController::GoToRandomTilePoint(): %s "), *point->GetName());
	//MoveToActor(point);
}

void AVaquerosAIController::UpdateTileToMove()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
}

void AVaquerosAIController::TaskMoveToTile()
{
	AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());

	//if(IsMyTurn() && pawn->IsMovementFinalized()){
	//Si no me estoy moviendo me muevo a la nueva casilla
	if (pawn->IsMovementFinalized()) {
		//if(!pawn->IsMoveInTurnCompleted()){
			UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
			FVector finalLocationVector = pBlackboardComponent->GetValueAsVector("TileToMove");

			LogicTile* tileToMove = VaquerosGameManager::GetInstance()->GetMatrix()->GetTile(finalLocationVector);
			pawn->MoveToTile(tileToMove);
			//pawn->SetMoveInTurnCompleted(true);
		//}
		//else {
		//}	
	}
}

bool AVaquerosAIController::UpdateObjetiveToShoot()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
	AVaquerosCharacter* pVaquerosCharacter = Cast<AVaquerosCharacter>(GetPawn());
	std::vector<LogicTile*> objetives = pVaquerosCharacter->objetivesInRange();
	int32 index = FMath::RandRange(0, objetives.size() - 1);
	if (objetives.size() > 0) {
		FVector ObjetiveLocation = VaquerosGameManager::GetInstance()->GetMatrix()->locationTile(objetives[index]);
		pBlackboardComponent->SetValueAsVector("ObjetiveLocation", ObjetiveLocation);
		return true;
	}
	return false;
}


bool AVaquerosAIController::TaskShootObjetive()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
	FVector vLocation = pBlackboardComponent->GetValueAsVector("ObjetiveLocation");
	AVaquerosCharacter* pVaquerosCharacter = Cast<AVaquerosCharacter>(GetPawn());
	LogicTile * tile = VaquerosGameManager::GetInstance()->GetMatrix()->GetTile(vLocation);

	if (tile && tile->getOccupant()) {
		return pVaquerosCharacter->makeShot(vLocation);
	}
	else {
		return false;
	}

}

bool AVaquerosAIController::IsMyTurn()
{
	AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
	VaquerosGameManager * vgmanager = VaquerosGameManager::GetInstance();

	return VaquerosGameManager::GetInstance()->IsEnemyInTurn(pawn) && !vgmanager->HaveEnemiesWon();
}

void AVaquerosAIController::TurnEnded()
{
	AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
	if (VaquerosGameManager::GetInstance()->EnemyTurnEnded(pawn)) {
		AVaquerosGameMode * gamemode = Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gamemode->ChangeTurn(false);
		gamemode->CheckNCOAAllPlayers();
	}
}

void AVaquerosAIController::TurnFinished()
{
	AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
	if (VaquerosGameManager::GetInstance()->FinishEnemyTurn(pawn)) {
		AVaquerosGameMode * gamemode = Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gamemode->ChangeTurn(false);
		gamemode->CheckNCOAAllPlayers();
	}
}

void AVaquerosAIController::MovementTurnEnded()
{
	if (IsMyTurn()) {
		UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
		AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
		VaquerosGameManager::GetInstance()->EndMovementTurn(pawn);
		pBlackboardComponent->SetValueAsBool("Attack", true);
	}
}

void AVaquerosAIController::AttackTurnEnded()
{
	if (IsMyTurn()) {
		UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();
		AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());
		VaquerosGameManager::GetInstance()->EndAttackTurn(pawn);
		pBlackboardComponent->SetValueAsBool("Attack", false);
	}
}

bool AVaquerosAIController::CheckTurn()
{
	AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());

	return VaquerosGameManager::GetInstance()->EnemyCheckTurn(pawn);
}

bool AVaquerosAIController::SelectAction()
{
	UBlackboardComponent* pBlackboardComponent = BrainComponent->GetBlackboardComponent();

	if (IsMyTurn()) {
		AVaquerosCharacter* pawn = Cast<AVaquerosCharacter>(GetPawn());	

		if (!pawn->IsMoveInTurnCompleted() && !pawn->IsAttackInTurnCompleted()) {

			std::vector<LogicTile*> objetives = pawn->objetivesInRange();
			int32 index = FMath::RandRange(0, objetives.size() - 1);
			if (objetives.size() > 0) {
				pBlackboardComponent->SetValueAsBool("Attack", true);
			}
			else {
				pBlackboardComponent->SetValueAsBool("Attack", false);
			}
		}
		else if (pawn->IsMoveInTurnCompleted() && !pawn->IsAttackInTurnCompleted()) {
			pBlackboardComponent->SetValueAsBool("Attack", true);
		}
		else if(!pawn->IsMoveInTurnCompleted() && pawn->IsAttackInTurnCompleted()) {
			pBlackboardComponent->SetValueAsBool("Attack", false);
		}
		else if (pawn->IsMoveInTurnCompleted() && pawn->IsAttackInTurnCompleted()) {
			// In this case, the turn should be finshed 
			pBlackboardComponent->SetValueAsBool("Attack", true);
		}
		return true;
	}
	
	return false;
	
}

AVaquerosCharacter* AVaquerosAIController::getClosestPlayer(AVaquerosCharacter* vaquerosPawn, Matriz* matrix)
{

	AVaquerosCharacter** players = VaquerosGameManager::GetInstance()->GetPlayers();
	AVaquerosCharacter* player = players[0];
	int min_distance = Pathfinding::FindPath(vaquerosPawn->GetVaqueroLocation(), player->GetVaqueroLocation(), matrix).size();

	for (int i = 1; i < VaquerosGameManager::GetInstance()->GetNumPlayers(); i++)
	{
		AVaquerosCharacter* aux_player = players[i];
		int distance = Pathfinding::FindPath(vaquerosPawn->GetVaqueroLocation(), aux_player->GetVaqueroLocation(), matrix).size();

		if (distance < min_distance && distance != 0)
		{
			min_distance = distance;
			player = aux_player;
		}
	}

	return player;
}
