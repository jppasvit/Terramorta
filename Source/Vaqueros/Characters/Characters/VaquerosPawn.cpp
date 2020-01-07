// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosPawn.h"

// Sets default values
AVaquerosPawn::AVaquerosPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVaquerosPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVaquerosPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVaquerosPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

