// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "VaquerosCameraActor.generated.h"

/**
 * 
 */
class ATargetPoint;
UCLASS()
class VAQUEROS_API AVaquerosCameraActor : public ACameraActor
{
	GENERATED_BODY()

private:
	// Determines if camera moves or not
	bool isTimeToMove = false;

	// Location of new target point
	FVector newLocation;

	// Moves camera based on DeltaTime
	void _MoveCamera(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	enum Position { LEFT, RIGHT };
	AVaquerosCameraActor();
	virtual void Tick(float DeltaTime) override;

	// Load target points of camera
	void LoadCameraTargetPoints();

	// Focus camera in central TargetPoint (visualReferencePoint)
	void FocusCamera();

	// Returns target point by Position
	ATargetPoint * FindTargetPoint(Position dir);

	// Returns current target point (TargetPoint of camera)
	ATargetPoint * CurrentTargetPoint();

	// Returns index of TargentPoint of camera
	int CurrentIndexTargetPoint();
	
	// Moves camera to other TargetPoint by Position
	void MoveCamera(Position dir);

	// Moves camera to other TargetPoint by bool value
	UFUNCTION(BlueprintCallable)
	void MoveCamera(bool dir);

	// TargetPoints of camera
	UPROPERTY(EditAnywhere)
	TArray<ATargetPoint *> positions;

	// TargetPoint for focus
	UPROPERTY(EditAnywhere)
	ATargetPoint * visualReferencePoint;

	// Tolerance for focus
	UPROPERTY(EditAnywhere)
	float alphaFocus = 1.f;

	// Camera position tolerance
	UPROPERTY(EditAnywhere)
	float currentPositionTolerance = 20.f;

	// Initial point of camera
	UPROPERTY(EditAnywhere)
	ATargetPoint * initialPoint;

	UPROPERTY(EditAnywhere)
	float speedMove = 300.f;

	



};
