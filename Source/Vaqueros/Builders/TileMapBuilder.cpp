// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMapBuilder.h"

#include <fstream>
#include <vector>
#include "JsonSerializer.h"
#include "PlatformFilemanager.h"
#include "FileHelper.h"
#include "Characters/Factory/VaquerosFactory.h"
using namespace std;

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameController/VaquerosGameManager.h"
#include "ConstructorHelpers.h"
#include "MatrixLogic/Matriz.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "VaquerosGameMode.h"
#include "Vaqueros/Characters/Characters/VaquerosCharacter.h"

FString ATileMapBuilder::_loadPath = "";

// Sets default values
ATileMapBuilder::ATileMapBuilder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FString dir = FString("Class'/Game/Blueprints/ShadowPlane.ShadowPlane_C'");
	ConstructorHelpers::FObjectFinder<UClass> myshadow(*dir);
	//_shadow = (UClass*)myBP.Object->GeneratedClass;
	_shadow = myshadow.Object;
	_matrix = nullptr;
}

// Called when the game starts or when spawned
void ATileMapBuilder::BeginPlay()
{
	Super::BeginPlay();

	//CreateMap();
	if (!_matrix) {
		UE_LOG(LogTemp, Warning, TEXT("Matrix es nullptr"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Matrix NO es nullptr"));
	}

	//AVaquerosGameMode * gamemode = Cast<AVaquerosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//VaquerosGameManager::GetInstance()->SetGameMode(gamemode);

	//VaquerosGameManager::GetInstance()->SetPlayers(_players);
	//VaquerosGameManager::GetInstance()->SetNumMaxPlayers(_numMaxPlayers);

}

void ATileMapBuilder::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	//Si el botón de guardado está activo no cargará mapas en ningún caso
	if (_saveMap) {
		SaveMap();
		_saveMap = false;
	}
	//Si se ha cambiado el tamano del mapa reseteará el mapa con el nuevo tamano
	else if(_generateNewMap)
	{
		newMap();
		_generateNewMap = false;
	}
	else
	{

		if (_loadMap) {
			CreateMap();
			_loadMap = false;
		}
	}


}

//Función encargada de crear un mapa vacío con los tamaños que se le indiquen en el editor
void ATileMapBuilder::newMap()
{
	DestroyMatrix();
	_NumTilesX = _Xeditor;
	_NumTilesY = _Yeditor;
	_matrix = new Matriz(_NumTilesX, _NumTilesY, 100, _offset);
	VaquerosGameManager::GetInstance()->SetMatrix(_matrix);

	for(int i = 0; i < _NumTilesX; i++)
	{
		for (int j = 0; j < _NumTilesY; j++)
		{
			CreateEmptyTile(i, j);
		}
	}

	TArray<AActor*> characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVaquerosCharacter::StaticClass(), characters);
	VaquerosGameManager::GetInstance()->SetVCharactersInGame(characters);
}


//Destruye tanto la mtriz lógica como la matriz del editor y los objetos que contenga
void ATileMapBuilder::DestroyMatrix()
{
	if(_tiles.Num())
	{
		while (_tiles.Num()) {
			_tiles.Pop()->Destroy();

		}
	}
	/*if (_matrix == nullptr)
	{
		_matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	}*/
	if (_matrix != nullptr)
	{

		for (int i = 0; i < _NumTilesX; ++i) {
			for (int j = 0; j < _NumTilesY; ++j) {
				delete _matrix->GetTile(i, j);
			}
		}


		delete _matrix;
		_matrix = nullptr;
	}


	//_matrix->~AMatriz();
}

// Called every frame
void ATileMapBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileMapBuilder::SetMaterial(int i, int j, UMaterial* material)
{
	_matrix->GetTile(i, j)->setMaterial(material);
	_tiles.Last()->SetPlaneMaterial(material);
}

//Carga un mapa desde fichero
void ATileMapBuilder::CreateMap()
{

	_loadPath = loadFileName;

	//Lee el json y comprueba que no esté vacío
	FString loadedJson = ReadJsonMap();

	if (loadedJson.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Error: File is empty."));
		return;
	}

	//Destruye la matriz que haya anteriormente
	DestroyMatrix();


	//Extrae el json comprobando que sea válido
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(loadedJson);

	if (!FJsonSerializer::Deserialize(Reader, jsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: The file has not valid data."));
		return;
	}

	//Inicializa la matriz y se la pasa al manager
	_NumTilesX = jsonObject->GetIntegerField("Rows");
	_NumTilesY = jsonObject->GetIntegerField("Columns");
	_Xeditor = _NumTilesX;
	_Yeditor = _NumTilesY;
	_matrix = new Matriz(_NumTilesX, _NumTilesY, 100.0f, _offset);
	VaquerosGameManager::GetInstance()->SetMatrix(_matrix);

	//Recoge el array de jsons que contiene las casillas
	TArray<TSharedPtr<FJsonValue>> tilesJSON = jsonObject->GetArrayField("Matrix");

	//Spawnea la factoría, posteriormente será eliminada y sólo se utiliza como ayuda. Guarda las UClass de los objetos que se tengan que spawnear.
	AVaquerosFactory* factory = GetWorld()->SpawnActor<AVaquerosFactory>(FVector(0, 0, 0), FRotator(0, 0, 0));
	TMap<FString, UClass*> classesBP = factory->getBPClasses();
	TMap<FString, UMaterial*> materialsBP = factory->getMaterialClasses();

	//Recorre las casillas, inicializándolas y creando el objeto que las ocupa si es necesario
	int i = 0, j = 0;
	for(TSharedPtr<FJsonValue> val : tilesJSON)
	{
		const TSharedPtr<FJsonObject>* occupant;
		const TSharedPtr<FJsonObject>* material;
		
		UClass* actorClass = nullptr;
		FString stringClass;
		FRotator Rotation(0.0f, 0.0f, 0.0f);

		if(val->AsObject()->TryGetObjectField("Occupant", occupant))
		{
			if (occupant->Get()->TryGetStringField("ClassName", stringClass))
			{


				stringClass.RemoveAt(0, 9);
				stringClass.InsertAt(0, "Class");
				if (!stringClass.Contains("_C"))
				{
					stringClass.InsertAt(stringClass.Len() - 1, "_C");
				}

				if (classesBP.Contains(stringClass))
				{
					actorClass = *classesBP.Find(stringClass);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Error: Class not found in the classes file."));
				}
			}

			double rotPitch, rotYaw, rotRoll;
			if(occupant->Get()->TryGetNumberField("RotationPitch", rotPitch) &&
				occupant->Get()->TryGetNumberField("RotationYaw", rotYaw) &&
				occupant->Get()->TryGetNumberField("RotationRoll", rotRoll))
			{
				Rotation = FRotator(rotPitch, rotYaw, rotRoll);
			}
		}
		
		
		FActorSpawnParameters SpawnInfo;
		AActor* occupantRef = nullptr;

		if (actorClass) {
			occupantRef = GetWorld()->SpawnActor<AActor>(actorClass, _matrix->locationTile(i, j), Rotation, SpawnInfo);
		}

		UMaterial* materialClass = nullptr;
		FString stringMaterialClass;

		if (val->AsObject()->TryGetObjectField("Material", material))
		{
			if (material->Get()->TryGetStringField("MaterialName", stringMaterialClass))
			{
				if (materialsBP.Contains(stringMaterialClass))
				{
					materialClass = *materialsBP.Find(stringMaterialClass);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Error: Class not found in the classes file."));
				}
			}
		}

		CreateTile(val, i, j, occupantRef);
		SetMaterial(i, j, materialClass);

		++j;
		if(j == _NumTilesY)
		{
			++i;
			j = 0;	
		}
	}

	factory->Destroy();

	TArray<AActor*> characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVaquerosCharacter::StaticClass(), characters);
	VaquerosGameManager::GetInstance()->SetVCharactersInGame(characters);
}

//Crea la casilla lógica y la casilla del editor relacionándolas
void ATileMapBuilder::CreateTile(const TSharedPtr<FJsonValue> json, int x, int y, AActor* occupant)
{
	//TSharedPtr<FJsonObject> tile = json->AsObject()->GetObjectField("Tile");
	//FActorSpawnParameters SpawnInfo;

	LogicTile* newULogicTile = new LogicTile(json->AsObject(), occupant, x, y, nullptr);
	_matrix->setTile(x, y, newULogicTile);

	/*FString aux = "Tile" + FString::FromInt(x) + "_" + FString::FromInt(y);
	SpawnInfo.Name = FName(*aux);*/
	ATile* newTile = GetWorld()->SpawnActor<ATile>(_matrix->locationTile(x, y), FRotator(0, 0, 0));
	newTile->SetCell(newULogicTile);
	_tiles.Add(newTile);

}

//Crea una casilla lógica y de editor vacía 
void ATileMapBuilder::CreateEmptyTile(int x, int y)
{
	FActorSpawnParameters SpawnInfo;
	LogicTile* newULogicTile = new LogicTile(nullptr);
	_matrix->setTile(x, y, newULogicTile);
	//_matrix->AddMatrix(newULogicTile);
	/*FString aux = "Tile" + FString::FromInt(x) + "xy" + FString::FromInt(y);
	SpawnInfo.Name = FName(*aux);*/
	ATile* newTile = GetWorld()->SpawnActor<ATile>(_matrix->locationTile(x, y), FRotator(0, 0, 0), SpawnInfo);
	newTile->SetCell(newULogicTile);
	_tiles.Add(newTile);
}
#if WITH_EDITOR
void ATileMapBuilder::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	FName propertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	UE_LOG(LogTemp, Warning, TEXT("Propiedad cambiada en TileMapBuilder: %s"), *propertyName.GetPlainNameString());
	//Change max numplayers
	if ((propertyName == GET_MEMBER_NAME_CHECKED(ATileMapBuilder, _numMaxPlayers)))
	{	
		VaquerosGameManager::GetInstance()->SetNumMaxPlayers(_numMaxPlayers);
		UE_LOG(LogTemp, Warning, TEXT("Ejecución de SetNumMaxPlayers"));
	}
	else if ((propertyName == GET_MEMBER_NAME_CHECKED(ATileMapBuilder, _numMaxEnemies)))
	{
		VaquerosGameManager::GetInstance()->SetNumMaxEnemies(_numMaxEnemies);
		UE_LOG(LogTemp, Warning, TEXT("Ejecución de SetNumMaxEnemies"));
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
//Guarda el mapa creado en un archivo json
void ATileMapBuilder::SaveMap() const
{

	//Comprueba si existe el  directorio
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString dir = FPaths::ProjectContentDir() + "\\SavedScenarios";

	// Directory Exists?
	if (!PlatformFile.CreateDirectoryTree(*dir))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: The directory doesn't exist."));
		return;
	}

	//Crea los objetos necesarios para usar el objeto json
	TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> tilesJSON;
	FString output;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&output);
	//AMatriz* matrix = VaquerosGameManager::GetInstance()->GetMatrix();
	Matriz* matrix = _matrix;

	json_object->SetNumberField("Columns", matrix->GetColumnas());
	json_object->SetNumberField("Rows", matrix->GetFilas());
	TSet<FString> blueprints;
	TSet<FString> materials;

	//Añade los json con los datos de las casillas 
	for(int i = 0; i < matrix->GetFilas(); ++i)
	{
		for (int j = 0; j < matrix->GetColumnas(); ++j)
		{
			LogicTile* tile = matrix->GetTile(i, j);
			tilesJSON.Add(MakeShareable(new FJsonValueObject(tile->toJson())));

			FString occupantBP = tile->getOccupantBP();
			if (!occupantBP.IsEmpty()) {
				blueprints.Add(occupantBP);
			}

			FString materialBP = tile->getMaterialBP();
			if (!materialBP.IsEmpty()) {
				materials.Add(materialBP);
			}
		}
	}

	json_object->SetArrayField("Matrix", tilesJSON);
	FJsonSerializer::Serialize(json_object.ToSharedRef(), Writer);
	//UE_LOG(LogTemp, Warning, TEXT("Matrix:  \n  %s"), *output);


	//Guarda el mapa en un fichero json
	FString path = dir + "\\" + _saveName + ".json";
	FFileHelper::SaveStringToFile(output, *path);

	//Guarda las BP que son utilizadas en este mapa
	FString pathBP = dir + "\\" + _saveName + ".text";
	FString stringBP;

	for(FString s : blueprints)
	{
		stringBP.Append(s + "\n");
	}

	FFileHelper::SaveStringToFile(stringBP, *pathBP);

	//Guarda los materiales que son utilizadas en este mapa
	FString pathMat = dir + "\\" + _saveName + "_materials.text";
	FString stringMat;

	for (FString s : materials)
	{
		stringMat.Append(s + "\n");
	}

	FFileHelper::SaveStringToFile(stringMat, *pathMat);
}

Matriz * ATileMapBuilder::GetMatrix() const
{
	return _matrix;
}

FString ATileMapBuilder::ReadJsonMap()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString dir = FPaths::ProjectContentDir();

	//#if !WITH_EDITOR
	dir += "SavedScenarios";
	//#endif

	FString path = dir + "\\" + _loadPath + ".json";

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: The file doesn't exist."));
		return FString();
	}

	FString textJson;

	FFileHelper::LoadFileToString(textJson, *path);

	return textJson;

}

FString ATileMapBuilder::getLoadPath()
{
	return _loadPath;
}



