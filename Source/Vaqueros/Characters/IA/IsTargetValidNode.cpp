// Fill out your copyright notice in the Description page of Project Settings.

#include "IsTargetValidNode.h"
#include "VaquerosAIController.h"

FString UIsTargetValidNode::GetStaticDescription() const
{
	return TEXT("Comprueba si el valor de la variable Target es válido, distinto de null y no referencia a un character destruido. Object Target");
}

EBTNodeResult::Type UIsTargetValidNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVaquerosAIController* AIController = Cast<AVaquerosAIController>(OwnerComp.GetOwner());
	if(AIController->isTargetValid())
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
