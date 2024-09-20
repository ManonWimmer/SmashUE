// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateCrouch.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateCrouch::GetStateID()
{
	return ESmashCharacterStateID::Crouch;
}

void USmashCharacterStateCrouch::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateCrouch")
		);

	Character->PlayAnimMontage(CrouchAnim);
}

void USmashCharacterStateCrouch::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateCrouch")
		);

	Character->UnCrouch();
}

void USmashCharacterStateCrouch::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetInputCrouchValue() > -0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}

void USmashCharacterStateCrouch::OnInputCrouch(float InputCrouchValue)
{
	Character->Crouch();
}

