// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <vector>
#include "GunBaseComponent.generated.h"

class AVaquerosCharacter;
class LogicTile;
UCLASS(Blueprintable, BlueprintType)
class VAQUEROS_API UGunBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunBaseComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool makeShot(LogicTile *source, LogicTile *destination);
	virtual std::vector<LogicTile*> tilesAffected(LogicTile *source);
	virtual int getAmmo() { return ammo; }
	virtual std::vector<LogicTile*> objetivesInRange(LogicTile *source, AVaquerosCharacter * shooter);
	virtual std::vector<LogicTile*> tilesInRange(LogicTile* source);
	virtual void Reload() { ammo = maxAmmo; }
	virtual bool canShoot() {/* return ammo > 0; */ return true; }
	virtual bool canShoot(LogicTile * source, LogicTile * destination);

private:
	AActor* searchObjetive(LogicTile* source, int incrX, int incrY, AVaquerosCharacter * shooter);
	void searchTilesInRange(LogicTile* source, int incrX, int incrY, std::vector<LogicTile*> &tilesInRange);
	int getIncr(int dir);

public:
	UPROPERTY(EditAnywhere)
		FString name;
	UPROPERTY(EditAnywhere)
		int damage = 1;
	UPROPERTY(EditAnywhere)
		int area = 0;
	UPROPERTY(EditAnywhere)
		int range = 3;
	UPROPERTY(EditAnywhere)
		int maxAmmo = 6;
private:
	int ammo;
		
};
