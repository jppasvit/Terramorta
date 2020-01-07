// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckTurnBTTaskNode.h"
#include "VaquerosAIController.h"
#include "GameController/VaquerosGameManager.h"

EBTNodeResult::Type UCheckTurnBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (controller->CheckTurn()) {
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;

}

FString UCheckTurnBTTaskNode::GetStaticDescription() const
{
	return TEXT("Comprueba si es el turno del jugador.");
}
