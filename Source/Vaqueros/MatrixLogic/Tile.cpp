// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane mesh"));
	auto meshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	_plane->SetupAttachment(RootComponent);
	_plane->SetStaticMesh(meshAsset.Object);
	_plane->SetWorldScale3D(FVector(1, 1, 1));

	if (meshAsset.Succeeded())
	{
		_plane->SetStaticMesh(meshAsset.Object);
		

		//if(FoundMaterial.Succeeded())
		//{
		//	_storedMaterial = FoundMaterial.Object;
		//}


		//_dynamicMaterialInst = UMaterialInstanceDynamic::Create(_storedMaterial, _plane);
		//_plane->SetMaterial(0, _dynamicMaterialInst); //Proximamente en On cursor over
		_plane->SetRenderCustomDepth(true);
	}

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::Destroyed()
{
	if(_cell && _cell->getOccupant())
	{
		_cell->getOccupant()->Destroy();
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::UpdateAttributes() {
	AActor * occupant = nullptr;
	if (_cell) {
		_cell->setWalkable(_walkableEditor);
		_cell->setJumpable(_jumpableEditor);
		_cell->setCost(_costEditor);
		_cell->_occupied = _occupiedEditor;

		AActor * oldOccupant = _cell->getOccupant();

		//Destruye el antiguo ocupante y setea como false la variable del editor por si no se crea otro ocupante
		if(oldOccupant)
		{
			oldOccupant->Destroy();
			_occupiedEditor = false;
		}

		//Spawnea el ocupante y se lo agrega a la casilla lógica 
		if(_occupantEditor)
		{
			FVector pos = GetActorLocation() + FVector(0, 0, 0); 
			occupant = GetWorld()->SpawnActor<AActor>(_occupantEditor, pos, FRotator(0, 0, 0));
			_cell->SetOccupant(occupant);
			_occupiedEditor = _cell->_occupied;
		}
		

		//Si la casilla está ocupada se notifica en el editor
		if (_occupiedEditor && _cell->getOccupant())
			_occupantEditor = _cell->getOccupant()->GetClass();
		else
			_occupantEditor = nullptr;


		if (_materialEditor)
		{
			UMaterialInstanceDynamic* material_dyn = UMaterialInstanceDynamic::Create(_materialEditor, this);
			_plane->SetMaterial(0, material_dyn);
		}
		_cell->setMaterial(_materialEditor);

	}
}

void ATile::OnConstruction(const FTransform & Transform) {
		UpdateAttributes();
}

void ATile::SetCell(LogicTile * _cell) {
	this->_cell = _cell;
	//UpdateAttributes();
	_costEditor = _cell->getCost();
	_jumpableEditor = _cell->_jumpable;
	if(_cell->_occupant)
		_occupantEditor = _cell->_occupant->GetClass();
	_occupiedEditor = _cell->_occupied;

}

void ATile::SetPlaneMaterial(UMaterial* material)
{
	if (material)
	{
		_materialEditor = material;
		UMaterialInstanceDynamic* material_dyn = UMaterialInstanceDynamic::Create(_materialEditor, this);
		_plane->SetMaterial(0, material_dyn);
	}
}
