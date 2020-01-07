// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameBuilder.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "GameController/VaquerosGameManager.h"
#include "VaquerosGameInstance.h"
#include "MatrixLogic/Matriz.h"
#include "Kismet/GameplayStatics.h"
#include "VaquerosGameMode.h"

AInGameBuilder::AInGameBuilder() {
	_floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (meshAsset.Succeeded())
	{
		_floor->SetupAttachment(RootComponent);
		_floor->SetStaticMesh(meshAsset.Object);
	}

}
void AInGameBuilder::BeginPlay()
{
	CreateMap();
	Matriz* Matriz = VaquerosGameManager::GetInstance()->GetMatrix();

	Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->setDayMode(_dayMode);

	_floor->SetWorldScale3D(FVector(Matriz->GetFilas(), Matriz->GetColumnas(), 1));
	_floor->SetWorldLocation(FVector(100* Matriz->GetFilas() / 2, 100* Matriz->GetColumnas() / 2, 0));
	_floor->SetMaterial(0, _floorMaterial);
	_floor->Mobility = EComponentMobility::Static;
	
	Cast<UVaquerosGameInstance>(GetWorld()->GetGameInstance())->SetPlayersHP(VaquerosGameManager::GetInstance()->GetPlayers(), VaquerosGameManager::GetInstance()->GetNumPlayers());
}

void AInGameBuilder::OnConstruction(const FTransform & Transform)
{
	_loadPath = loadFileName;
}

void AInGameBuilder::CreateTile(const TSharedPtr<FJsonValue> json, int x, int y, AActor* occupant)
{
	//TSharedPtr<FJsonObject> tile = json->AsObject()->GetObjectField("Tile");
	//FActorSpawnParameters SpawnInfo;
	
	FVector shadowlocation = _matrix->locationTile(x, y);
	shadowlocation += FVector(0, 0, 0.00001);
	AActor *shadow = GetWorld()->SpawnActor<AActor>(_shadow, shadowlocation, FRotator(0, 0, 0));

	LogicTile* newULogicTile = new LogicTile(json->AsObject(), occupant, x, y, shadow);
	_matrix->setTile(x, y, newULogicTile);

	/*FString aux = "Tile" + FString::FromInt(x) + "_" + FString::FromInt(y);
	SpawnInfo.Name = FName(*aux);*/
	/*ATile* newTile = GetWorld()->SpawnActor<ATile>(_matrix->locationTile(x, y), FRotator(0, 0, 0));
	newTile->SetCell(newULogicTile);
	_tiles.Add(newTile);*/
	

}

void AInGameBuilder::SetMaterial(int i, int j, UMaterial* material)
{

}
