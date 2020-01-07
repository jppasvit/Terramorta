// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IsTargetValidNode.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API UIsTargetValidNode : public UBTTaskNode
{
	GENERATED_BODY()

	FString GetStaticDescription() const override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
