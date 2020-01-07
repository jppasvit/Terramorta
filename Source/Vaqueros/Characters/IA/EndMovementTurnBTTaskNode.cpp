// Fill out your copyright notice in the Description page of Project Settings.

#include "EndMovementTurnBTTaskNode.h"
#include "VaquerosAIController.h"


EBTNodeResult::Type UEndMovementTurnBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());
	controller->MovementTurnEnded();

	return EBTNodeResult::Succeeded;
}

FString UEndMovementTurnBTTaskNode::GetStaticDescription() const
{
	return FString("Nodo que finaliza el turno de movimiento del enemigo");
}