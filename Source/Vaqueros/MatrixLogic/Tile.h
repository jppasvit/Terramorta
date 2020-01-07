// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "LogicTile.h"
#include "Tile.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VAQUEROS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* _plane;


	UPROPERTY(EditAnywhere)
	bool _walkableEditor = true;

	UPROPERTY(EditAnywhere)
	bool _jumpableEditor = true;

	UPROPERTY(EditAnywhere)
	int _costEditor = 1;

	UPROPERTY(EditAnywhere)
	bool _occupiedEditor = false;

	UPROPERTY(EditAnywhere)
	UClass *_occupantEditor = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterial *_materialEditor = nullptr;

	LogicTile * _cell = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform & Transform)override;
	virtual void SetCell(LogicTile * _cell);
	void SetPlaneMaterial(UMaterial* material);

private:
	void UpdateAttributes();
};
