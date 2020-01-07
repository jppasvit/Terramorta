// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SelectActionBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API USelectActionBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

	USelectActionBTTaskNode(const FObjectInitializer& objectInitializer);

	//Se llama al iniciar este task
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
	
};
