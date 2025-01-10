// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCameraManager.h"

#include "AdvancedCharMovementComponent.h"
#include "MovementCharacter.h"
#include "Components/CapsuleComponent.h"

ACharacterCameraManager::ACharacterCameraManager()
{
}

void ACharacterCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
	
	if (AMovementCharacter* MovementCharacter = Cast<AMovementCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UAdvancedCharMovementComponent* AMC = MovementCharacter->GetAdvancedCharacterMovementComponent();
		
		FVector TargetCrouchOffset = FVector(0,0, AMC->GetCrouchedHalfHeight() - MovementCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CameraBlendDuration, 0.0f, 1.0f));

		if (AMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.0f, CameraBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.0f, 1.0f);
		}

		if (AMC->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
	
}
