// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateIdle.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::OnInputCrouch(float InputCrouch)
{
	if (InputCrouch < -0.1f)
		StateMachine->ChangeState(ESmashCharacterStateID::Crouch);
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateIdle")
		);
	
	Character->PlayAnimMontage(IdleAnim);
	
	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputCrouchEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputCrouch);
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputJump);
}

void USmashCharacterStateIdle::OnInputJump(float InputJump)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateIdle")
		);

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputCrouchEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputCrouch);
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputJump);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (!Character->GetCharacterMovement()->IsMovingOnGround()) StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	
	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXTreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}


