// Fill out your copyright notice in the Description page of Project Settings.

#include "SearchEnemyBTTaskNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type USearchEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AVaquerosAIController* AIController = Cast<AVaquerosAIController>(OwnerComp.GetOwner());
	if (AIController->UpdateObjetiveToShoot()) {
		return EBTNodeResult::Succeeded;
	}
	//return EBTNodeResult::Failed;
	return EBTNodeResult::Succeeded;
}

FString USearchEnemyBTTaskNode::GetStaticDescription() const
{
	return  TEXT("Elige objetivo al azar al que disparar. Vector ObjetiveLocation (objetivo al que disparar).");
}
