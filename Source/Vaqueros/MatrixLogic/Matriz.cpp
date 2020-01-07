// Fill out your copyright notice in the Description page of Project Settings.

#include "Matriz.h"
#include "Engine/World.h"

// Sets default values
Matriz::Matriz()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	matriz = nullptr;
	_numFilas = 0;
	_numColumnas = 0;
	_tamTile = 0.f;
	_offset = 0.f;
	_hFloor = 0;
}

Matriz::Matriz(int _filas, int _columnas, float _tam, float _offset, float _hFloor)
{

	matriz = new LogicTile*[_filas*_columnas];
	for (int i = 0; i < _filas*_columnas; ++i) matriz[i] = nullptr;
	_numFilas = _filas;
	_numColumnas = _columnas;
	_tamTile = _tam;
	this->_offset = _offset;
	this->_hFloor = _hFloor;
}

LogicTile * Matriz::GetTile(int _x, int _y)
{
	if (_x < 0 || _x >= _numFilas || _y < 0 || _y >= _numColumnas) {
		return nullptr;
	}
	return matriz[_x*_numColumnas + _y];
}

LogicTile * Matriz::GetTile(FVector coord)
{
	return GetTile(getCoor(coord.X), getCoor(coord.Y));
}

FVector Matriz::locationTile(int _x, int _y)
{
	float x, y;
	x = _tamTile / 2 + _x * (_tamTile + _offset);
	y = _tamTile / 2 + _y * (_tamTile + _offset);
	return FVector(x, y, _hFloor);
}


FVector Matriz::locationTile(const LogicTile* tile)
{
	float _x = tile->getLogicalX(), _y = tile->getLogicalY(), x, y;
	x = _tamTile / 2 + _x * (_tamTile + _offset);
	y = _tamTile / 2 + _y * (_tamTile + _offset);
	return FVector(x, y, _hFloor);
}

bool Matriz::setTile(int _x, int _y, LogicTile * _tile, FRotator _rotation)
{
	if (_x < 0 || _x >= _numFilas || _y<0 || _y>_numColumnas)
		return false;

	delete matriz[_x*_numColumnas + _y];
	matriz[_x*_numColumnas + _y] = _tile;

	_tile->SetCoordinates(_x, _y);
	return true;
}

int Matriz::getCoor(float coord)
{
	if (coord > 0)
		return round((coord - (_tamTile) / 2) / (_tamTile + _offset));

	return ceil(coord - (_tamTile) / 2) / (_tamTile + _offset);
}

void Matriz::shine(std::vector<LogicTile*> vector, bool mode, int selectDecal)
{
	for (auto tile : vector)
		tile->shine(mode, selectDecal);
}

TArray<LogicTile*> Matriz::neighbours(int _x, int _y)
{
	TArray<LogicTile*> neighbours;
	if (_y + 1 < _numColumnas) {
		neighbours.Push(GetTile(_x, _y + 1));
	}

	if (_y - 1 >= 0) {
		neighbours.Push(GetTile(_x, _y - 1));
	}

	if (_x - 1 >= 0) {
		neighbours.Push(GetTile(_x - 1, _y));
	}

	if (_x + 1 < _numFilas) {
		neighbours.Push(GetTile(_x + 1, _y));
	}

	return neighbours;
}

int Matriz::GetFilas()
{
	return _numFilas;
}

int Matriz::GetColumnas()
{
	return _numColumnas;
}

Matriz::~Matriz()
{
	delete[] matriz;
}