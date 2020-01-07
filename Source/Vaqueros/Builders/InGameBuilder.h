// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileMapBuilder.h"
#include "InGameBuilder.generated.h"

/**
 * 
 */
UCLASS()
class VAQUEROS_API AInGameBuilder : public ATileMapBuilder
{
	GENERATED_BODY()
public:
	AInGameBuilder();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void CreateTile(const TSharedPtr<FJsonValue> jsonTile, int x, int y, AActor* occupant) override;

	UStaticMeshComponent* _floor;

	UPROPERTY(EditAnywhere)
	UMaterial* _floorMaterial;

	UPROPERTY(EditAnywhere)
	bool _dayMode = true;

	UFUNCTION(BlueprintCallable)
	bool GetDayMode() { return _dayMode; }

private:
	void SetMaterial(int i, int j, UMaterial* material) override;
	
};
