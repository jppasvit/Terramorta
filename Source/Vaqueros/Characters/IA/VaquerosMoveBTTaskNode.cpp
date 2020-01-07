// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosMoveBTTaskNode.h"
#include "VaquerosAIController.h"
#include "GameController/VaquerosGameManager.h"
#include "Characters/Characters/VaquerosCharacter.h"

UVaquerosMoveBTTaskNode::UVaquerosMoveBTTaskNode(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UVaquerosMoveBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UVaquerosMoveBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());
	

	controller->TaskMoveToTile();

		//AVaquerosCharacter * enemy = VaquerosGameManager::GetInstance()->GetEnemyInTurn(); // TEMPORAL PARA PRUEBAS
		//enemy->SetAttackInTurnCompleted(true); // TEMPORAL PARA PRUEBAS
		//enemy->SetTurnCompleted(true); // TEMPORAL PARA PRUEBAS
	
	if (controller->CharacterMovementFinalized()) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
		

}

FString UVaquerosMoveBTTaskNode::GetStaticDescription() const
{
	return TEXT("Mueve el character a la tile que se le indique siguiendo el camino determinado por el pathfinding. Vector TileToMove (casilla a la que se moverá).");
}
