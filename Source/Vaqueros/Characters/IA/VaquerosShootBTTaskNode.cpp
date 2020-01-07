// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosShootBTTaskNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type UVaquerosShootBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AVaquerosAIController* AIController = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (AIController->TaskShootObjetive()) {
		return EBTNodeResult::Succeeded;
	}

	//return EBTNodeResult::Failed;
	return EBTNodeResult::Succeeded;
}

FString UVaquerosShootBTTaskNode::GetStaticDescription() const
{
	return TEXT("Dispara al objetivo. Vector ObjetiveLocation (Posición del objetivo a disparar).");
}
