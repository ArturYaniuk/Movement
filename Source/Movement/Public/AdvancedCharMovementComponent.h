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
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_WallRun		UMETA(DisplayName = "WallRun"),
	CMOVE_Hang 			UMETA(DisplayName = "Hang"),
	CMOVE_Climb			UMETA(DisplayName = "Climb"),
	CMOVE_MAX			UMETA(Hidden),
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
		uint8 Saved_bWantsToDash : 1;
		uint8 Saved_bPressedMovementJump : 1;
		
		// Other Variables
		uint8 Saved_bPrevWantsToCrouch : 1;
		uint8 Saved_bWantsToProne : 1;
		uint8 Saved_bHadAnimRootMotion : 1;
		uint8 Saved_bTransitionFinished : 1;
		uint8 Saved_bWallRunIsRight : 1;

		
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
	UPROPERTY(EditDefaultsOnly, Category = Parameters) float MaxSprintSpeed = 700.f;
	UPROPERTY(EditDefaultsOnly, Category = Parameters) float Walk_MaxWalkSpeed = 250.f;

	// Slide
	UPROPERTY(EditDefaultsOnly, Category = Slide) float MinSlideSpeed = 350.f;
	UPROPERTY(EditDefaultsOnly, Category = Slide) float MaxSlideSpeed=350.f;
	UPROPERTY(EditDefaultsOnly, Category = Slide) float SlideEnterImpulse = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = Slide) float SlideGravityForce = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category = Slide) float SlideFrictionFactor = .06f;
	UPROPERTY(EditDefaultsOnly, Category = Slide) float BrakingDecelerationSliding=1000.f;

	// Prone
	UPROPERTY(EditDefaultsOnly, Category = Prone) float ProneEnterHoldDuration = .2f;
	UPROPERTY(EditDefaultsOnly, Category = Prone) float ProneSlideEnterImpulse = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = Prone) float MaxProneSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = Prone) float BreakingDecelerationsProning = 2500.f;

	// Dash
	UPROPERTY(EditDefaultsOnly, Category = Dash) float DashCooldownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = Dash) float AutoDashCooldownDuration = .9f;
	UPROPERTY(EditDefaultsOnly, Category = Dash) UAnimMontage* DashMontage;

	// Mantle
	UPROPERTY(EditDefaultsOnly, Category = Mantle) float MantleMaxDistance = 200;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) float MantleReachHeight = 50;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) float MinMantleDepth = 30;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) float MantleMinWallSteepnessAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) float MantleMaxSurfaceAngle = 40;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) float MantleMaxAlignmentAngle = 45;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) UAnimMontage* TallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) UAnimMontage* TransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) UAnimMontage* ProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Mantle) UAnimMontage* ProxyShortMantleMontage;

	// Wall Run
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float MinWallRunSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float MaxWallRunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float MaxVerticalWallRunSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float WallRunPullAwayAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float WallAttractionForce = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float MinWallRunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) UCurveFloat* WallRunGravityScaleCurve;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float WallJumpOffForce = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = WallRun) float GravityAcceleration = -300.f;
	
	
	
	// Hang
	UPROPERTY(EditDefaultsOnly, Category = Hang) UAnimMontage* TransitionHangMontage;
	UPROPERTY(EditDefaultsOnly, Category = Hang) UAnimMontage* WallJumpMontage;
	UPROPERTY(EditDefaultsOnly, Category = Hang) float WallJumpForce = 400.f;

	// Climb
	UPROPERTY(EditDefaultsOnly, Category = Climb) float MaxClimbSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = Climb) float BreakingDecelerationClimbing = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = Climb) float ClimbReachDistance = 200.f;
	
	UPROPERTY(Transient)	AMovementCharacter* MovementCharacterOwner;

	// Flags
	
	bool Safe_bWantsToSprint;
	bool Safe_bWantsToProne;
	bool Safe_bWantsToDash;

	bool Safe_bHadAnimRootMotion;
	bool Safe_bPrevWantsToCrouch;
	float DashStartTimer;
	FTimerHandle TimerHandle_EnterProne;
	FTimerHandle TimerHandle_DashCooldown;

	bool Safe_bTransitionFinished;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
	FString TransitionName;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;

	bool Safe_bWallRunIsRight;

	// Replication
	UPROPERTY(ReplicatedUsing=OnRep_DashStart) bool Proxy_bDashStart;

	UPROPERTY(ReplicatedUsing=OnRep_ShortMantle) bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing=OnRep_TallMantle) bool Proxy_bTallMantle;

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

	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves, float DeltaTime) override;

protected:

	virtual void InitializeComponent() override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

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

	// Mantle
	bool TryMantle();
	FVector GetMantleStartLocation(FHitResult& FrontHit, FHitResult SurfaceHit, bool bTallMantle) const;
	
	// Wall Run
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);

	// Try Climb

	bool TryHang();

	bool TryClimb();
	void PhysClimb(float deltaTime, int32 Iterations);

	// Helpers
	bool IsServer() const;
	float CapR() const;
	float CapHH() const;

public:
	
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UFUNCTION(BlueprintCallable) void DashPressed();
	UFUNCTION(BlueprintCallable) void DashReleased();

	UFUNCTION(BlueprintCallable) void ClimbPressed();
	UFUNCTION(BlueprintCallable) void ClimbReleased();

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;

	UFUNCTION(BlueprintPure) bool IsWallRunning() const { return IsCustomMovementMode(CMOVE_WallRun); };
	UFUNCTION(BlueprintPure) bool WallRunningIsRight() const { return Safe_bWallRunIsRight; };
	UFUNCTION(BlueprintPure) bool IsHanging() const { return IsCustomMovementMode(CMOVE_Hang); }
	UFUNCTION(BlueprintPure) bool IsClimbing() const { return IsCustomMovementMode(CMOVE_Climb); }

	
	// Proxy Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION() void OnRep_DashStart();
	UFUNCTION() void OnRep_ShortMantle();
	UFUNCTION() void OnRep_TallMantle();
};
