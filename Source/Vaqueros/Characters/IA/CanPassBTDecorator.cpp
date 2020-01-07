// Fill out your copyright notice in the Description page of Project Settings.

#include "CanPassBTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UCanPassBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{

	const UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
	if (blackboardComp)
	{
		if (blackboardComp->GetValueAsBool(blackboardKey.SelectedKeyName) == boolVariable) {
			return true;
		}
	}
	return false;
}

FString UCanPassBTDecorator::GetStaticDescription() const
{
	return FString("Determina si con la variable de paso del BlackBoard, se permite continuar");
}
