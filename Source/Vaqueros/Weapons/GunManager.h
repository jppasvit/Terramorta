// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MatrixLogic/Matriz.h"
#include "MatrixLogic/LogicTile.h"
#include <vector>
#include "GunManager.generated.h"
class UGunBaseComponent;
class AVaquerosCharacter;
//class UBasicAbility;
/*
	Componente encargado de manejar las armas equpadas de cada personaje.
	Contiene la lista estatica de todas las armas del juego, el arma equipada
	y el cd de todas las armas del personaje.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VAQUEROS_API UGunManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	// Sets default values for this component's properties
	UGunManager();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UGunBaseComponent *Gun;
	//UBasicAbility * Ability;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Funcion que ejecuta el disparo del arma seleccionada.
	virtual bool makeShot(LogicTile *source, LogicTile *destination);
	//Funcion que dice si el disparo se puede ejecutar.
	virtual bool canShoot();
	//Funcion que dice si el disparo se puede ejecutar.
	virtual bool canShoot(LogicTile *source, LogicTile *destination);
	//Funcion que devuelve la lista de los jugadores a tiro del equipo contrario desde la posicion dada.
	virtual std::vector<LogicTile*> objetivesInRange(LogicTile *source, AVaquerosCharacter * shooter);
	//Función que devuelve las casillas al alcance de un arma en una posición
	virtual std::vector<LogicTile*> tilesInRange(LogicTile *source);
	//Funcion que recarga el arma equipada.
	virtual void reload();
	//Se deja para que no falle. Hay que borrarla
	void myTick();
	/*
	//Funcion que ejecuta la habilidad desde la casilla source hasta la casilla destination.
	virtual bool ExecuteAbility(LogicTile *source, LogicTile *destination);
	//Función que devuelve las casillas a las que afectaria la habilidad al ejecutarla desde source
	virtual std::vector<LogicTile*> tilesInAbilityRange(LogicTile *source);
	*/
};
