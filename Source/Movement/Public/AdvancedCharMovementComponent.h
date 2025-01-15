// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedCharMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(Hidden),
	CMOVE_Slide UMETA(DisplayName = "Slide"),
	CMOVE_Prone UMETA(DisplayName = "Prone"),
	CMOVE_MAX UMETA(Hidden),
};

UCLASS()
class MOVEMENT_API UAdvancedCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	// ReSharper disable once CppClassNeedsConstructorBecauseOfUninitializedMember
	class FSavedMove_Movement : public FSavedMove_Character
	{
	public:
		enum CompressedFlags
		{
			FLAG_Sprint     = 0x10,
			FLAG_Dash	    = 0x20,
			FLAG_Custom_2   = 0x40,
			FLAG_Custom_3   = 0x80,
		};

	
		// Flags
		uint8 Saved_bWantsToSprint : 1;
		uint8 Saved_bWantsToDash :1;
		
		// Other Variables
		uint8 Saved_bPrevWantsToCrouch: 1;
		uint8 Saved_bWantsToProne: 1;

		
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
	UPROPERTY(EditDefaultsOnly) float MaxSprintSpeed = 700.f;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed = 250.f;

	// Slide
	UPROPERTY(EditDefaultsOnly) float MinSlideSpeed = 350.f;
	UPROPERTY(EditDefaultsOnly) float MaxSlideSpeed=350.f;
	UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse = 500.f;
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce = 5000.f;
	UPROPERTY(EditDefaultsOnly) float SlideFrictionFactor = .06f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding=1000.f;

	// Prone
	UPROPERTY(EditDefaultsOnly) float ProneEnterHoldDuration = .2f;
	UPROPERTY(EditDefaultsOnly) float ProneSlideEnterImpulse = 300.f;
	UPROPERTY(EditDefaultsOnly) float MaxProneSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly) float BreakingDecelerationsProning = 2500.f;

	// Dash
	UPROPERTY(EditDefaultsOnly) float DashImpulse = 2000.f;
	UPROPERTY(EditDefaultsOnly) float DashCooldownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly) float AutoDashCooldownDuration = .9f;
	UPROPERTY(EditDefaultsOnly) UAnimMontage* DashMontage;


	
	UPROPERTY(Transient)	AMovementCharacter* MovementCharacterOwner;

	// Flags
	
	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToProne;
	bool Safe_bWantsToDash;

	float DashStartTimer;
	
	FTimerHandle TimerHandle_EnterProne;
	FTimerHandle TimerHandle_DashCooldown;

	// Replication
	UPROPERTY(ReplicatedUsing=OnRep_DashStart) bool Proxy_bDashStart;

	//	Delegates
public:
	
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;
	
	
	UAdvancedCharMovementComponent();

	// Network stuff
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	//Character Movement Component
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

protected:

	virtual void InitializeComponent() override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;


private:
	// Slide
	
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool CanSlide() const;

	// Prone

	void TryEnterProne() { Safe_bWantsToProne = true; }
	UFUNCTION(Server, Reliable) void Server_EnterProne();
	
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float deltaTime, int32 Iterations);

	
	// Dash
	void OnDashCooldownFinished();
	
	bool CanDash() const;
	void PerformDash();


public:
	
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UFUNCTION(BlueprintCallable) void DashPressed();
	UFUNCTION(BlueprintCallable) void DashReleased();

	

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION() void OnRep_DashStart();
};
