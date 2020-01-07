// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LogicTile.h"
#include "CoreMinimal.h"
#include <vector>

class Matriz
{

public:
	// Sets default values for this actor's properties
	Matriz();
	Matriz(int _filas, int _columnas, float _tam, float _offset, float _hFloor = 0.0f);

	~Matriz();

protected:

private:
	LogicTile** matriz;
	int _numFilas, _numColumnas;
	float _tamTile, _offset, _hFloor;
	FVector _initialPos;

public:

	int GetFilas();

	int GetColumnas();


	TArray<LogicTile*> neighbours(int _x, int _y);

	LogicTile* GetTile(int _x, int _y);

	LogicTile* GetTile(FVector coord);

	FVector locationTile(int _x, int _y);

	FVector locationTile(const LogicTile* tile);

	bool setTile(int _x, int _y, LogicTile* _tile, FRotator _rotation = FRotator(0.0f, 0.0f, 0.0f));

	int getCoor(float coord);

	void shine(std::vector<LogicTile*> vector, bool mode, int selectDecal);
};
