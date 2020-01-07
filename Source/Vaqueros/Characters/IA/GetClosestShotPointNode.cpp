// Fill out your copyright notice in the Description page of Project Settings.

#include "GetClosestShotPointNode.h"
#include "VaquerosAIController.h"

EBTNodeResult::Type UGetClosestShotPointNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVaquerosAIController* controller = Cast<AVaquerosAIController>(OwnerComp.GetOwner());

	if (controller->GetClosestShotPoint())
	{
		return  EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

FString UGetClosestShotPointNode::GetStaticDescription() const
{
	return TEXT("Selecciona la casilla m�s cercana desde la que disparar al Target o al enemigo m�s cercano (Target = Null). Si no existe posici�n desde la que disparar se acerca a �l. Object Target");
}
