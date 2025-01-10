// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CharacterCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API ACharacterCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float CameraBlendDuration = .5;
	
	float CrouchBlendTime;

public:
	ACharacterCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
