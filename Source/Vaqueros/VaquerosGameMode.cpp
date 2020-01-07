// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosGameMode.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Characters/Characters/VaquerosCharacter.h"
#include "Vaqueros/Characters/Controllers/VaquerosPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "VaquerosCameraActor.h"
#include "Vaqueros/MatrixLogic/LogicTile.h"
#include "Utils/Pathfinding.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Vaqueros/Weapons/GunManager.h"
#include <vector>


AVaquerosGameMode::AVaquerosGameMode() {
	FString dir = FString("WidgetBlueprint'/Game/Assets/HUD/VaquerosHUD.VaquerosHUD_C'");
	ConstructorHelpers::FObjectFinder<UClass> hudWidgetObj(*dir);
	_VaquerosHudClass = hudWidgetObj.Object;
}

void AVaquerosGameMode::BeginPlay()
{
	_VaquerosHud = CreateWidget<UUserWidget>(GetGameInstance()->GetFirstLocalPlayerController(), _VaquerosHudClass);
	_VaquerosHud->AddToViewport();
	TArray<AActor*> cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVaquerosCameraActor::StaticClass(), cameras);
	_VaquerosCamera = Cast<AVaquerosCameraActor>(cameras[0]);
}

void AVaquerosGameMode::StartPlay()
{
	Super::StartPlay();
	CheckNCOAAllPlayers();
}

void AVaquerosGameMode::UpdateCharacter(AVaquerosCharacter * actualCharacter)
{
	FOutputDeviceNull  ar;
	FString command = TEXT("ActualizarImagen ");
	command.Append(actualCharacter->GetName());
	_VaquerosHud->CallFunctionByNameWithArguments(*command, ar, _VaquerosHud, true);
	UE_LOG(LogTemp, Warning, TEXT("UpdateCharacter %s "), *actualCharacter->GetName());
}

void AVaquerosGameMode::ChangeTurn(bool enemyTurn)
{
	FOutputDeviceNull  ar;
	const FString command = FString::Printf(TEXT("ChangeTurn %d"), enemyTurn);
	_VaquerosHud->CallFunctionByNameWithArguments(*command, ar, _VaquerosHud, true);
	UE_LOG(LogTemp, Warning, TEXT("ChangeTurn %d"), enemyTurn);
}

void AVaquerosGameMode::PassSubTurn()
{
	AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
	playercontroller->PassTurn();
}

void AVaquerosGameMode::PassTurn()
{
	AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());

	if( playercontroller->CheckCanClickAnyWhere() ){
		VaquerosGameManager * vgminstance = VaquerosGameManager::GetInstance();
		Matriz* matrix = vgminstance->GetMatrix();
		AVaquerosCharacter * currentCharacter = Cast<AVaquerosCharacter>(vgminstance->GetCurrentCharacter());

		if (currentCharacter) {
			LogicTile* fromTile = currentCharacter->GetVaqueroLocation();

			currentCharacter->GlowCharacter(false);

			vector<LogicTile*> path = Pathfinding::FindNearTiles(fromTile, currentCharacter->GetNumMovements(), matrix);
			matrix->shine(path, false, 2);

			path = currentCharacter->objetivesInRange(fromTile);
			matrix->shine(path, false, 1);
			currentCharacter->ShowTilesInRangeForAttack(false);
		}

		AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
		AVaquerosCharacter * selectedEnemy = playercontroller->GetSelectedEnemy();
		if ( !playercontroller->IsSelectedEnemyDestroyed() && selectedEnemy && IsValid(selectedEnemy) && IsValid(selectedEnemy) ) {
			selectedEnemy->ShowAllTiles(false);
		}

		vgminstance->SetCurrentCharacterAndInTurn(nullptr);

		vgminstance->EndTurnPlayers();
		this->ChangeTurn(true);
	}
}

bool AVaquerosGameMode::CanShowHoverMoveDecal(AActor * shadow)
{
	if (shadow) {
		AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playercontroller->GetSelectedEnemy() && playercontroller->IsTilesSelectedEnemyIlluminated()) {
			return false;
		}

		TArray<UDecalComponent*> childrens;
		shadow->GetComponents<UDecalComponent>(childrens);

		for (int i = 0; i < childrens.Num(); i++)
		{
			if (childrens[i] && childrens[i]->IsVisible() && childrens[i]->GetName().Equals(FString("MoveDecal"))) {
				return true;
			}
		}
	}
	return false;
}

bool AVaquerosGameMode::CanShowHoverShootDecal(AActor * shadow)
{
	if (shadow) {
		AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playercontroller->GetSelectedEnemy() && playercontroller->IsTilesSelectedEnemyIlluminated()) {
			return false;
		}

		TArray<UDecalComponent*> childrens;
		shadow->GetComponents<UDecalComponent>(childrens);

		for (int i = 0; i < childrens.Num(); i++)
		{
			if (childrens[i] && childrens[i]->IsVisible() && childrens[i]->GetName().Equals(FString("ShootDecal"))) {
				return true;
			}
		}
	}
	return false;
}

void AVaquerosGameMode::ShowShootingRangeCursorOver(AActor * shadow, bool mode)
{
	if (shadow) {
		AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playercontroller->GetSelectedEnemy() && playercontroller->IsTilesSelectedEnemyIlluminated()) {
			return;
		}

		VaquerosGameManager * vgminstance = VaquerosGameManager::GetInstance();
		AVaquerosCharacter * player = Cast<AVaquerosCharacter>(vgminstance->GetCurrentCharacter());

		if ( player && !player->IsAttackInTurnCompleted() && player->IsMovementFinalized() && !player->IsMoveInTurnCompleted()) {
			//VaquerosGameManager::GetInstance()->LogCharacter(player);
			//UE_LOG(LogTemp, Error, TEXT("Player is IsMovementFinalized: %d"), player->IsMovementFinalized());
			if ( mode ) {
				TArray<UDecalComponent*> childrens;
				shadow->GetComponents<UDecalComponent>(childrens);

				for (int i = 0; i < childrens.Num(); i++)
				{
					if (childrens[i] && childrens[i]->IsVisible() && childrens[i]->GetName().Equals(FString("MoveDecal"))) {
						Matriz* matrix = vgminstance->GetMatrix();
						LogicTile * tile = matrix->GetTile(shadow->GetActorLocation());
						if (tile) {
							vector<LogicTile*> path = player->_gunManagerComponent->tilesInRange(tile);
							matrix->shine(path, mode, 5);
							path = player->objetivesInRange(tile);
							matrix->shine(path, mode, 1);
						}
					}
				}
			}
			else {
				Matriz* matrix = vgminstance->GetMatrix();
				LogicTile * tile = matrix->GetTile(shadow->GetActorLocation());
				if (tile) {
					vector<LogicTile*> path = player->_gunManagerComponent->tilesInRange(tile);
					matrix->shine(path, mode, 5);
					path = player->objetivesInRange(tile);
					matrix->shine(path, mode, 1);
				}
				player->ShowTilesForAttack(true);
				player->ShowTilesInRangeForAttack(true);
			}
		}
	}
	
}

AVaquerosCharacter * AVaquerosGameMode::GetCurrentCharacter()
{
	return Cast<AVaquerosCharacter>(VaquerosGameManager::GetInstance()->GetCurrentCharacter());
}

AVaquerosCharacter * AVaquerosGameMode::GetVCharacterByCharacterName(FString name)
{
	return VaquerosGameManager::GetInstance()->GetVCharacterByCharacterName(name);
}

bool AVaquerosGameMode::SetPlayerInTurnByCharacterName(FString name)
{
	VaquerosGameManager * vgminstance = VaquerosGameManager::GetInstance();
	AVaquerosPlayerController * playercontroller = Cast<AVaquerosPlayerController>(GetWorld()->GetFirstPlayerController());

	if ( vgminstance->CanBlueprintSetPlayerInTurnByCharacterName && playercontroller->CheckCanClickAnyWhere() ) {
		AVaquerosCharacter * player = vgminstance->GetVCharacterByCharacterName(name);
		if (player && vgminstance->GetIndexPlayer(player) != -1 && (!player->IsMoveInTurnCompleted() || !player->IsAttackInTurnCompleted()) ) {

			AVaquerosCharacter * currentCharacter = Cast<AVaquerosCharacter>(vgminstance->GetCurrentCharacter());
			if (currentCharacter) {
				currentCharacter->ShowAllTiles(false);
				currentCharacter->GlowCharacter(false);
			}

			vgminstance->SetCurrentCharacterAndInTurn(player);

			if (playercontroller->GetSelectedEnemy()) {
				playercontroller->GetSelectedEnemy()->ShowAllTiles(false);
			}

			//player->ShowAllTiles(true);
			if ( !player->IsMoveInTurnCompleted() ) {
				player->ShowTilesInRange(true);
			}

			if ( !player->IsAttackInTurnCompleted() ) {
				player->ShowTilesForAttack(true);
				player->ShowTilesInRangeForAttack(true);
			}
			
			player->GlowCharacter(true);

			return true;
		}
	}
	return false;
}

bool AVaquerosGameMode::IsCharacterTurnCompleted(AVaquerosCharacter * character)
{
	return VaquerosGameManager::GetInstance()->IsCharacterTurnCompleted(character);
}

bool AVaquerosGameMode::IsCharacterActionsCompleted(AVaquerosCharacter * character)
{
	return VaquerosGameManager::GetInstance()->IsCharacterActionsCompleted(character);
}

void AVaquerosGameMode::NotifyChangesOfAttributes(AVaquerosCharacter * character)
{
	FOutputDeviceNull  ar;
	FString command = TEXT("NotifyChangesOfAttributes ");
	command.Append(character->GetName());
	_VaquerosHud->CallFunctionByNameWithArguments(*command, ar, _VaquerosHud, true);
	//UE_LOG(LogTemp, Warning, TEXT("NotifyChangesOfAttributes %s "), *character->GetName());
}

void AVaquerosGameMode::CheckNCOAAllPlayers()
{
	VaquerosGameManager * vgminstance = VaquerosGameManager::GetInstance();
	for (int i = 0; i < vgminstance->GetNumPlayers(); i++)
	{
		NotifyChangesOfAttributes(vgminstance->GetPlayer(i));
	}
}

void AVaquerosGameMode::setCamera(AVaquerosCameraActor * camera)
{
	_VaquerosCamera = camera;
}

void AVaquerosGameMode::setDayMode(bool dayMode)
{
	_dayMode = dayMode;
	_dayModeCorrect = true;
}
