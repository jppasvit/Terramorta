// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CanPassBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API UCanPassBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

protected:

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector blackboardKey;

	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool boolVariable;

public:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	
};
