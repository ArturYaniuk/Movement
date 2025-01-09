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

	if (Saved_bWantsToSprint) Result = FLAG_Custom_0;

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

UAdvancedCharMovementComponent::FNetworkPredictionData_Client_Movement::FNetworkPredictionData_Client_Movement(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{
	
}

FSavedMovePtr UAdvancedCharMovementComponent::FNetworkPredictionData_Client_Movement::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Character());
}

FNetworkPredictionData_Client* UAdvancedCharMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);
	
	if (ClientPredictionData != nullptr)
	{
		UAdvancedCharMovementComponent* MutableThis = const_cast<UAdvancedCharMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Movement(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
		
	}
	return ClientPredictionData;
}

void UAdvancedCharMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UAdvancedCharMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

void UAdvancedCharMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UAdvancedCharMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

void UAdvancedCharMovementComponent::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;
}

UAdvancedCharMovementComponent::UAdvancedCharMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}
