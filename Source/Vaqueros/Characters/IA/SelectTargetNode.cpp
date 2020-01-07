 // Fill out your copyright notice in the Description page of Project Settings.

#include "SelectTargetNode.h"
#include "VaquerosAIController.h"

 EBTNodeResult::Type USelectTargetNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AVaquerosAIController* AIController = Cast<AVaquerosAIController>(OwnerComp.GetOwner());
	if (AIController->UpdateTarget()) {
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

FString USelectTargetNode::GetStaticDescription() const
{
	return  TEXT("Elige un objetivo en el que centrarse. Object Target (objetivo en el que se centra).");
}