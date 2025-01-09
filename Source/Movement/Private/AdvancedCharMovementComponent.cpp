// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedCharMovementComponent.h"

#include "GameFramework/Character.h"

bool UAdvancedCharMovementComponent::FSavedMove_Movement::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Movement* NewMovementMove = static_cast<FSavedMove_Movement*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewMovementMove ->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UAdvancedCharMovementComponent::FSavedMove_Movement::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UAdvancedCharMovementComponent::FSavedMove_Movement::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result != FLAG_Custom_0;

	return Result;
}

void UAdvancedCharMovementComponent::FSavedMove_Movement::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UAdvancedCharMovementComponent* CharacterMovement = Cast<UAdvancedCharMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UAdvancedCharMovementComponent::FSavedMove_Movement::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UAdvancedCharMovementComponent* CharacterMovement = Cast<UAdvancedCharMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UAdvancedCharMovementComponent::UAdvancedCharMovementComponent()
{
}
