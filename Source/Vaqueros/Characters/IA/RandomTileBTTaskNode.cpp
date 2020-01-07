// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomTileBTTaskNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type URandomTileBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (controller->GetRandomTilePoint())
	{
		return  EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

FString URandomTileBTTaskNode::GetStaticDescription() const
{
	return TEXT("Selecciona una tile aleatoria dentro del rango del personaje y a la que sea posible moverse y setea al controller el path. Vector TileToMove (casilla elegida)");
}
