// Fill out your copyright notice in the Description page of Project Settings.

#include "GetBestPositionBTTaskNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type UGetBestPositionBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (controller->GetBestTilePoint())
	{
		return  EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

FString UGetBestPositionBTTaskNode::GetStaticDescription() const
{
	return TEXT("Selecciona la casilla que tenga mayor número de enemigos a tiro.");
}