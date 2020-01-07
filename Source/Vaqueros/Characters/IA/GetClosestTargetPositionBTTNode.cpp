// Fill out your copyright notice in the Description page of Project Settings.

#include "GetClosestTargetPositionBTTNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type UGetClosestTargetPositionBTTNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (controller->GetClosestEnemyTilePoint())
	{
		return  EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

FString UGetClosestTargetPositionBTTNode::GetStaticDescription() const
{
	return TEXT("Selecciona la casilla más cercana al enemigo más cercano.");
}