// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectActionBTTaskNode.h"
#include "VaquerosAIController.h"


USelectActionBTTaskNode::USelectActionBTTaskNode(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type USelectActionBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (controller->SelectAction()) {
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

FString USelectActionBTTaskNode::GetStaticDescription() const
{
	return FString("Selector de acciones");
}
