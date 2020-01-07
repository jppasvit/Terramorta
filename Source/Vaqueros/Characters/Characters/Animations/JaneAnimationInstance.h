// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JaneAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API UJaneAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Stop = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RotateLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RotateRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool receiveDamage = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool shootAnim = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool dieAnim = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* objetiveToShoot = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool turnAround = false;

	UFUNCTION(BlueprintCallable)
		void UpdateProperties();
};
