// Fill out your copyright notice in the Description page of Project Settings.

#include "VaquerosCameraActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


AVaquerosCameraActor::AVaquerosCameraActor() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVaquerosCameraActor::BeginPlay()
{
	Super::BeginPlay();

	//LoadCameraTargetPoints();
	
	if (initialPoint) {
		this->SetActorLocation(initialPoint->GetActorLocation());
		FocusCamera();
	}

}

void AVaquerosCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isTimeToMove) {
		this->_MoveCamera(DeltaTime);
	}
}

void AVaquerosCameraActor::MoveCamera(Position dir)
{
	
	int currentTarget = this->CurrentIndexTargetPoint();
	int newPosition = 0;

	if (dir == Position::LEFT) {
		newPosition = currentTarget - 1;
	}
	else if (dir == Position::RIGHT) {
		newPosition = currentTarget + 1;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MoveCamera: Position doesn't exist"));
		return;
	}
	if(!isTimeToMove){
		int numPositions = positions.Num();
		if ( newPosition >= -1 && newPosition <= numPositions ) {
			if (newPosition == -1) {
				newPosition = numPositions - 1;
			}
			else if ( newPosition == numPositions ) {
				newPosition = 0;
			}

			ATargetPoint * newtargetpoint = positions[newPosition];
			if (newtargetpoint) {
				newLocation = newtargetpoint->GetActorLocation();
				isTimeToMove = true;
			}
		}
	}
}

void AVaquerosCameraActor::MoveCamera(bool dir)
{
	if (dir) {
		this->MoveCamera(LEFT);
	}
	else {
		this->MoveCamera(RIGHT);
	}
}

void AVaquerosCameraActor::_MoveCamera(float DeltaTime)
{
	FVector currentLocation = this->GetActorLocation();
	FVector direction = newLocation - currentLocation;
	direction.Normalize();
	this->SetActorLocation(currentLocation + (direction * speedMove * DeltaTime), true);
	this->FocusCamera();

	if (currentLocation.Equals(newLocation, currentPositionTolerance)) {
		isTimeToMove = false;
	}
}

void AVaquerosCameraActor::LoadCameraTargetPoints()
{
	TArray<AActor *> targetpositions;
	UGameplayStatics::GetAllActorsOfClass(GEngine->GetWorld(), ATargetPoint::StaticClass(), targetpositions);

	for(AActor * targetposition : targetpositions)
	{
		ATargetPoint * targetpoint = Cast<ATargetPoint>(targetposition);
		if (targetpoint != visualReferencePoint && targetpoint != initialPoint) {
			positions.Push(Cast<ATargetPoint>(targetpoint));
		}
	}
}

void AVaquerosCameraActor::FocusCamera()
{
	FRotator interrot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), visualReferencePoint->GetActorLocation());
	FRotator finalrot = FMath::Lerp(this->GetActorRotation(), interrot, alphaFocus);
	this->SetActorRotation(finalrot);

	//this->SetActorRotation(visualReferencePoint->Get)
}

// TO-DO 
ATargetPoint * AVaquerosCameraActor::FindTargetPoint(Position dir)
{
/*	ATargetPoint * returnpoint;
	FVector currentLocation = this->GetActorLocation();

	if (dir == Position::LEFT) {
		ATargetPoint * temppoint;
		for (ATargetPoint * targetposition : positions)
		{
			
			FVector directionVector = targetposition->GetActorLocation() - currentLocation;
			float dotproduct = FVector::DotProduct(targetposition->GetActorRightVector(), directionVector);
			
		}
	}
	else if (dir == Position::RIGHT) {

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MoveCamera: Position doesn't exist"));
	}*/
	return nullptr;
}

ATargetPoint * AVaquerosCameraActor::CurrentTargetPoint()
{
	FVector currentLocation = this->GetActorLocation();
	for (ATargetPoint * targetposition : positions)
	{
		if (currentLocation.Equals(targetposition->GetActorLocation(), currentPositionTolerance)) {
			return targetposition;
		}
	}
	
	return nullptr;
}

int AVaquerosCameraActor::CurrentIndexTargetPoint()
{
	FVector currentLocation = this->GetActorLocation();
	int targetindex = 0;
	for (ATargetPoint * targetposition : positions)
	{
		if (currentLocation.Equals(targetposition->GetActorLocation(), currentPositionTolerance)) {
			return targetindex;
		}
		targetindex++;
	}

	return -1;
}
