// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateJump")
		);
	Character->PlayAnimMontage(JumpAnim);
	OnInputJump();
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateJump")
		);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	FVector Velocity = Character->GetVelocity();
	float ZVelocity = Velocity.Z;

	if (ZVelocity < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}

	Character->SetOrientX(Character->GetInputMoveX());
	Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());

	CalculateJumpVelocity();
	
}

void USmashCharacterStateJump::OnInputJump()
{
    float JumpVelocity = (2.0f * JumpMaxHeight) / (JumpDuration / 2.0f);
	
    FVector JumpImpulse = FVector(0.0f, 0.0f, JumpVelocity);
    Character->LaunchCharacter(JumpImpulse, false, true);
	
    JumpLocation = Character->GetActorLocation();
	
    float Gravity = (-8.0f * JumpMaxHeight) / FMath::Square(JumpDuration); 
    Character->GetCharacterMovement()->GravityScale = FMath::Abs(Gravity / GetWorld()->GetGravityZ());
}

void USmashCharacterStateJump::CalculateJumpVelocity()
{
    FVector Velocity = Character->GetVelocity();
    float ZVelocity = Velocity.Z;
	
    if (Character->GetActorLocation().Z - JumpLocation.Z > JumpMaxHeight)
    {
        FVector NewVelocity = Velocity;
        NewVelocity.Z = 0.0f;
        Character->GetCharacterMovement()->Velocity = NewVelocity;
        StateMachine->ChangeState(ESmashCharacterStateID::Fall);
    }
}



