// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
using namespace std;
#include "MatrixLogic/Tile.h"
#include "TileMapBuilder.generated.h"

class AVaquerosCharacter;
class Matriz;

UCLASS()
	class VAQUEROS_API ATileMapBuilder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATileMapBuilder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void newMap();
	virtual FString ReadJsonMap();
	virtual void DestroyMatrix();
	virtual void CreateTile(const TSharedPtr<FJsonValue> jsonTile, int x, int y, AActor* occupant);
	virtual void CreateEmptyTile(int x, int y);
	virtual void SetMaterial(int i, int j, UMaterial* material);


	UClass* _shadow;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//Choose the tile class depending on the code read
	//virtual ATile* ParseCode(string code, int _x, int _y);

	TMap<FString, UClass*> _objectClasses;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateMap();
	void SaveMap() const;
	static FString getLoadPath();

	// Called every frame
	UPROPERTY(EditAnyWhere)
		FString loadFileName="";

	static FString _loadPath;

	UPROPERTY(EditAnyWhere)
	FString _saveName = "";

	UPROPERTY(EditAnywhere)
	int _Xeditor;
	int _NumTilesX = 0;

	UPROPERTY(EditAnywhere)
	int _Yeditor;
	int _NumTilesY = 0;

	UPROPERTY(EditAnywhere)
	float _offset;

	UPROPERTY(EditAnywhere)
	bool _loadMap = false;

	UPROPERTY(EditAnywhere)
	bool _saveMap = false;

	UPROPERTY(EditAnywhere)
	bool _generateNewMap = false;

	UPROPERTY(EditAnywhere)
	int _numMaxPlayers = 8;

	UPROPERTY(EditAnywhere)
	int _numMaxEnemies = 15;

	UPROPERTY(BlueprintReadOnly)
	TArray<ATile*> _tiles;

	Matriz* GetMatrix() const;

	Matriz* _matrix;





};
