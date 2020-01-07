// Fill out your copyright notice in the Description page of Project Settings.

#include "EndTurnBTTaskNode.h"
#include "VaquerosAIController.h"
#include "GameController/VaquerosGameManager.h"
#include "Characters/Characters/VaquerosCharacter.h"

EBTNodeResult::Type UEndTurnBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	//controller->TurnEnded();
	if(!controller->IsMyTurn())
	{
		return EBTNodeResult::Failed;
	}

	controller->TurnFinished();

	return EBTNodeResult::Succeeded;
}

FString UEndTurnBTTaskNode::GetStaticDescription() const
{
	return FString("Nodo que finaliza el turno de los enemigos o hace que avence el turno");
}
