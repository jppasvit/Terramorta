// Fill out your copyright notice in the Description page of Project Settings.

#include "EndAttackTurnBTTaskNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type UEndAttackTurnBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());
	controller->AttackTurnEnded();


	return EBTNodeResult::Succeeded;
}

FString UEndAttackTurnBTTaskNode::GetStaticDescription() const
{
	return FString("Nodo que finaliza el turno de ataque del enemigo");
}
