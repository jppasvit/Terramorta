// Fill out your copyright notice in the Description page of Project Settings.

#include "MatrixLogic/LogicTile.h"
#include "Characters/Characters/VaquerosActor.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"


/*ULogicTile::ULogicTile(AActor *_shadow)
{
	this->_walkable = true;
	this->_jumpable = false;
	this->_cost = 1;
	this->_occupant = nullptr;
	this->_occupied = _occupant;
	this->_shadow = _shadow;
}

ULogicTile::ULogicTile(TSharedPtr<FJsonObject> jsonTile, AActor* occupant ,int x, int y, AActor *_shadow)
{
	_x = x;
	_y = y;
	_walkable = jsonTile->GetBoolField("Walkable");
	//_jumpable = jsonTile->GetBoolField("Jumpable");
	_cost = jsonTile->GetBoolField("Cost");
	this->_occupant = occupant;
	this->_occupied = occupant;
	this->_shadow = _shadow;
}

ULogicTile::ULogicTile(bool _walkable, bool _jumpable, int _cost, AActor * _occupant)
{
	this->_walkable = _walkable;
	this->_jumpable = _jumpable;
	this->_cost = _cost;
	this->_occupant = _occupant;
	this->_occupied = _occupant;
}*/
LogicTile::LogicTile() {

}

LogicTile::~LogicTile()
{
	if (_occupant)
		_occupant->Destroy();
	if (this->_shadow)
		this->_shadow->Destroy();
}


LogicTile::LogicTile(AActor * _shadow)
{
	this->_walkable = true;
	this->_jumpable = false;
	this->_cost = 1;
	this->_occupant = nullptr;
	this->_occupied = false;
	this->_shadow = _shadow;
}

LogicTile::LogicTile(TSharedPtr<FJsonObject> jsonTile, AActor * occupant, int x, int y, AActor * _shadow)
{
	_x = x;
	_y = y;
	_walkable = jsonTile->GetBoolField("Walkable");
	//_jumpable = jsonTile->GetBoolField("Jumpable");
	_cost = jsonTile->GetBoolField("Cost");
	this->_occupant = occupant;
	if (_occupant)
		this->_occupied = true;
	else
		this->_occupied = jsonTile->GetBoolField("Occupied");
	this->_shadow = _shadow;
}

LogicTile::LogicTile(bool _walkable, bool _jumpable, int _cost, AActor * _occupant)
{
	this->_walkable = _walkable;
	this->_jumpable = _jumpable;
	this->_cost = _cost;
	this->_occupant = _occupant;
	if (_occupant)
		this->_occupied = true;
	else
		this->_occupied = false;
}

void LogicTile::SetOccupant(AActor * object)
{
	_occupant = object;
	if (_occupant)
		this->_occupied = true;
	else
		this->_occupied = false;
}

AActor * LogicTile::getOccupant()
{
	return _occupant;
}

int LogicTile::getCost()
{
	return _cost;
}

int LogicTile::getLogicalX() const
{
	return _x;
}

int LogicTile::getLogicalY() const
{
	return _y;
}

void LogicTile::SetCoordinates(int x, int y)
{
	_x = x;
	_y = y;
}

void LogicTile::setMaterial(UMaterial* material)
{
	_material = material;
}

TSharedPtr<FJsonObject> LogicTile::toJson()
{

	TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
	json_object->SetBoolField("Walkable", _walkable);
	json_object->SetNumberField("Cost", _cost);
	json_object->SetBoolField("Occupied", _occupied);
	if (AVaquerosActor* saveOcuppant = dynamic_cast<AVaquerosActor*>(_occupant))
	{
		TSharedPtr<FJsonObject> json_occupant = MakeShareable(new FJsonObject);
		FString className = "Blueprint'" + _occupant->GetClass()->GetPathName() + "'";
		json_occupant->SetStringField("ClassName", className); //LeftChop quita "_C"
		json_object->SetObjectField("Occupant", json_occupant);
		json_occupant->SetNumberField("RotationPitch", _occupant->GetActorRotation().Pitch);
		json_occupant->SetNumberField("RotationYaw", _occupant->GetActorRotation().Yaw);
		json_occupant->SetNumberField("RotationRoll", _occupant->GetActorRotation().Roll);
	}

	if (_material)
	{
		TSharedPtr<FJsonObject> json_material = MakeShareable(new FJsonObject);
		FString className = "Material'" + _material->GetPathName() + "'";
		json_material->SetStringField("MaterialName", className); //LeftChop quita "_C"
		json_object->SetObjectField("Material", json_material);
	}

	return json_object;
	//TO-DO: crear estándar para guardar los objetos que ocupan la casilla
	//JsonObject->SetObjectField("Occupant", _occupant.);
}

FString LogicTile::getOccupantBP()
{
	if (_occupant)
		return "Class'" + _occupant->GetClass()->GetPathName() + "'";

	return "";
}

FString LogicTile::getMaterialBP()
{
	if (_material)
		return "Material'" + _material->GetPathName() + "'";;

	return "";
}

void LogicTile::shine(bool mode, int selectDecal)
{
	FOutputDeviceNull  ar;
	//std::string life = std::to_string(_hp);
	FString command;
	/* FString command = FString::Printf(TEXT("ChangeColor %d"), Attack);
   _shadow->CallFunctionByNameWithArguments(*command, ar, _shadow, true);
   _shadow->GetRootComponent()->SetVisibility(mode);*/
	if (selectDecal == 1) {
		command = FString::Printf(TEXT("ShowShoot %d"), mode);
		_shadow->CallFunctionByNameWithArguments(*command, ar, _shadow, true);
	}
	else if (selectDecal == 2) {
		command = FString::Printf(TEXT("ShowMove %d"), mode);
		_shadow->CallFunctionByNameWithArguments(*command, ar, _shadow, true);
	}
	else if (selectDecal == 3) {
		command = FString::Printf(TEXT("ShowHoverMove %d"), mode);
		_shadow->CallFunctionByNameWithArguments(*command, ar, _shadow, true);
	}
	else if (selectDecal == 4) {
		command = FString::Printf(TEXT("ShowHoverShoot %d"), mode);
		_shadow->CallFunctionByNameWithArguments(*command, ar, _shadow, true);
	}
	else if (selectDecal == 5) {
		command = FString::Printf(TEXT("ShowShootRange %d"), mode);
		_shadow->CallFunctionByNameWithArguments(*command, ar, _shadow, true);
	}
}
