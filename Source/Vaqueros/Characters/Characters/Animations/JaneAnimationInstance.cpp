// Fill out your copyright notice in the Description page of Project Settings.

#include "JaneAnimationInstance.h"
#include "Characters/Characters/VaquerosCharacter.h"
void UJaneAnimationInstance::UpdateProperties()
{
	AActor* pMyActor = GetOwningActor();
	AVaquerosCharacter* pCharacter = Cast<AVaquerosCharacter>(pMyActor);
	if (pCharacter) {
		pCharacter->UpdateAnimationVariables(Stop, RotateLeft, RotateRight, receiveDamage, shootAnim, objetiveToShoot, dieAnim, turnAround);
	}
}
