// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedCharMovementComponent.generated.h"


UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(Hidden),
	CMOVE_Slide UMETA(DisplayName = "Slide"),
	CMOVE_MAX UMETA(Hidden),
};

UCLASS()
class MOVEMENT_API UAdvancedCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Movement : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint : 1;

		uint8 Saved_bPrevWantsToCrouch: 1;


	public:
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Movement : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Movement(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
	//Parameters
	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed = 700.f;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 350.f;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 500.f;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000.f;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3f;
	
	UPROPERTY(Transient)
	AMovementCharacter* MovementCharacterOwner;

	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;
		
public:
	
	UAdvancedCharMovementComponent();
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

protected:

	virtual void InitializeComponent() override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;


	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit);

public:
	
	UFUNCTION(BlueprintCallable) 
	void SprintPressed();
	
	UFUNCTION(BlueprintCallable)
	void SprintReleased();

	UFUNCTION(BlueprintCallable)
	void CrouchPressed();

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
};
