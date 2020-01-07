// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SearchEnemyBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API USearchEnemyBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual FString GetStaticDescription() const override;
};
