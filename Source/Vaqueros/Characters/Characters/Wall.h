// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VaquerosActor.h"
#include "Wall.generated.h"

UCLASS(Blueprintable)
class VAQUEROS_API AWall : public AVaquerosActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool _throwOver = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
