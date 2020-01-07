// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosActor.h"
#include <typeinfo>

// Sets default values
AVaquerosActor::AVaquerosActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVaquerosActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVaquerosActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVaquerosActor::fromJson()
{
}

TSharedPtr<FJsonObject> AVaquerosActor::toJson()
{
	TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
	json_object->SetStringField("Type", typeid(this).name());

	return json_object;
}

