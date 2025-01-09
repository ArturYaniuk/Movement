// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UAdvancedCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Movement : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint : 1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	bool Safe_bWantsToSprint;
	
public:
	UAdvancedCharMovementComponent();

};
