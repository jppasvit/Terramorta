// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Core/Public/HAL/FileManager.h"
#include "Characters/Characters/VaquerosCharacter.h"
#include "Engine.h"
#include <string>

UVaquerosGameInstance::UVaquerosGameInstance() {
	FString dir = FString("WidgetBlueprint'/Game/Assets/HUD/YouWin_Widget.YouWin_Widget_C'");
	ConstructorHelpers::FObjectFinder<UClass> hudWidgetObj(*dir);
	_victoryMenu = hudWidgetObj.Object;
	dir = FString("WidgetBlueprint'/Game/Assets/HUD/YouLose_Widget.YouLose_Widget_C'");
	ConstructorHelpers::FObjectFinder<UClass> hudWidgetObj1(*dir);
	_defeatMenu = hudWidgetObj1.Object;
	TArray<FString> Files;
	FString path = FPaths::ProjectContentDir();
	IFileManager::Get().FindFilesRecursive(Files, *path, TEXT("*.umap"), true, false, false);
	for (FString name : Files) {
		if (name.Contains("GameLevel_")) {
			name.RemoveFromStart(*path);
			name.RemoveFromEnd(".umap");
			name.InsertAt(0, "/Game/");
			MapFiles.Add(name);
		}
	}
}

void UVaquerosGameInstance::startMap(int map, int subLevel=0)
{
	std::string dir = "/Game/Maps/GameLevel_" + std::to_string(map)+"_SubLevel_"+ std::to_string(subLevel);
	//std::string dir = "/Game/Maps/GameLevel_" + std::to_string(map);
	FString name(dir.c_str());
	FName mapName = FName(*name);
	_currentMap = map;
	_gameManager->SetToNull();
	UGameplayStatics::OpenLevel(this, mapName);

}

void UVaquerosGameInstance::notifyEndGame(bool result)
{
	if (result) {
		//launch win menu
		CreateWidget<UUserWidget>(this->GetFirstLocalPlayerController(), _victoryMenu)->AddToViewport();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("true"));
		saveHPCharacters();
	}
	else {
		//launch lost menu
		CreateWidget<UUserWidget>(this->GetFirstLocalPlayerController(), _defeatMenu)->AddToViewport();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("false"));
	}
}

void UVaquerosGameInstance::backToWelcomeMenu()
{
	std::string dir = "/Game/Maps/MainMenu";
	FString name(dir.c_str());
	FName mapName = FName(*name);
	UGameplayStatics::OpenLevel(this, mapName);
}

void UVaquerosGameInstance::startNextMap()
{
#if WITH_EDITOR
	FString CurrentMapName = GetWorld()->GetMapName();
	FString currentSubLevel, currentLevel, aux;
	CurrentMapName.Split("GameLevel_", &currentSubLevel, &aux);
	aux.Split("_SubLevel_",&currentLevel, &currentSubLevel);
	_currentMap = FCString::Atoi(*currentLevel);
	_currentSubLevel = FCString::Atoi(*currentSubLevel);
#endif
	FName mapName;
	std::string dir = "/Game/Maps/GameLevel_" + std::to_string(++_currentMap) + "_SubLevel_" + std::to_string(_currentSubLevel);
	FString name(dir.c_str());
	if (!MapFiles.Contains(name)) {
		_currentMap = 0;
		dir = "/Game/Maps/GameLevel_" + std::to_string(_currentMap) + "_SubLevel_" + std::to_string(++_currentSubLevel);
		name = dir.c_str();
		if (!MapFiles.Contains(name)) {
			//No quedan mapas.
			backToWelcomeMenu();
			return;
		}
	}
	
	/*std::string dir = "/Game/Maps/GameLevel_" + std::to_string(++_currentMap);
	FString name(dir.c_str());*/
	mapName = FName(*name);
	_gameManager->SetToNull();
	UGameplayStatics::OpenLevel(this, mapName);
}

void UVaquerosGameInstance::restartMap()
{
#if WITH_EDITOR
	FString CurrentMapName = GetWorld()->GetMapName();
	FString currentSubLevel, currentLevel, aux;
	CurrentMapName.Split("GameLevel_", &currentSubLevel, &aux);
	aux.Split("_SubLevel_", &currentLevel, &currentSubLevel);
	_currentMap = FCString::Atoi(*currentLevel);
	_currentSubLevel = FCString::Atoi(*currentSubLevel);
#endif
	startMap(_currentMap, _currentSubLevel);
}

void UVaquerosGameInstance::SetPlayersHP(AVaquerosCharacter** characters, int num)
{
//#if WITH_EDITOR
//	FString CurrentMapName = GetWorld()->GetMapName();
//	FString currentSubLevel, currentLevel, aux;
//	CurrentMapName.Split("GameLevel_", &currentSubLevel, &aux);
//	aux.Split("_SubLevel_", &currentLevel, &currentSubLevel);
//	_currentMap = FCString::Atoi(*currentLevel);
//	_currentSubLevel = FCString::Atoi(*currentSubLevel);
//	if(!HPCharacters.Num())
//		startNewRound(characters, num);
//	
//#endif

	if (_currentMap == 0) {
		startNewRound(characters, num);
	}
	else {
		startSublevel(characters, num);
	}
}

void UVaquerosGameInstance::UpdateHPCharacter(AVaquerosCharacter* character, int hp)
{
	HPCharacters[character->GetNameCharacter()] = hp;
}


void UVaquerosGameInstance::UpdateAllHPCharacters(AVaquerosCharacter** characters, int num)
{
	for (int i = 0; i < num; ++i) {
		//En realidad sera HPCharacters[Characters[i]->GetIndexGame] o algo asi.
		HPCharacters.Add(characters[i]->GetNameCharacter(),characters[i]->GetHp());
		//Set index in character
		//characters[i]->SetGameIndex(i);
	}
}

void UVaquerosGameInstance::NotifyDead(AVaquerosCharacter * character)
{
	charactersAlive[character->GetNameCharacter()] = false;
}

void UVaquerosGameInstance::startNewRound(AVaquerosCharacter** characters, int num)
{
	//SetIndex in characters.
	HPCharacters.Empty();
	charactersAlive.Empty();	
	for (int i = 0; i < num; ++i) {
		HPCharacters.Add(characters[i]->GetNameCharacter(), characters[i]->GetHp());
		charactersAlive.Add(characters[i]->GetNameCharacter(), true);
		//Set i in character as Index in Game
		//characters[i]->SetGameIndex(i);
	}
}

void UVaquerosGameInstance::startSublevel(AVaquerosCharacter** characters, int num)
{
	TArray<AVaquerosCharacter*> charTokill;
	for (int i = 0; i < num; i++) {
		if (!HPCharacters.Contains(characters[i]->GetNameCharacter())) {
			HPCharacters.Add(characters[i]->GetNameCharacter(), characters[i]->GetHp());
			HPCharacters.Add(characters[i]->GetNameCharacter(), true);
		}
		else {
			//characters[i]->_hp = HPCharacters[characters[i]->GetNameCharacter()];
			characters[i]->SetHp(HPCharacters[characters[i]->GetNameCharacter()]);
			charactersAlive.Add(characters[i]->GetNameCharacter(), true);
			characters[i]->updateHud();
			//lo mismo que el comentario de antes
			if (HPCharacters[characters[i]->GetNameCharacter()]<=0) {
				charTokill.Push(characters[i]);
			}
		}
	}
	for (AVaquerosCharacter* character : charTokill) {
		//Funcion para eliminar el jugador de la partida (al ser antes de que empiece sin animaciones ni nada. simplemente que desaparezca).
		VaquerosGameManager::GetInstance()->DestroyVaquerosCharacter(character);
	}
}

void UVaquerosGameInstance::saveHPCharacters()
{
	AVaquerosCharacter** lista = VaquerosGameManager::GetInstance()->GetPlayers();
	int num = VaquerosGameManager::GetInstance()->GetNumPlayers();

	for (int i = 0; i < num; ++i) {
		HPCharacters.Add(lista[i]->GetNameCharacter(), lista[i]->GetHp());
		//HPCharacters[lista[i]->GetGameIndex()] = lista[i]->_hp;
	}
	for (auto i : charactersAlive) {
		if (!i.Value) {
			HPCharacters.Add(i.Key, 0);
		}
	}
}
