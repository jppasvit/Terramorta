// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "VaquerosMoveBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API UVaquerosMoveBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

	UVaquerosMoveBTTaskNode(const FObjectInitializer& objectInitializer);
	//Se llama al iniciar este task
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//Se llama constantemente y es usado en tasks que en el execute task devuelven InProgress
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual FString GetStaticDescription() const override;
	
};
