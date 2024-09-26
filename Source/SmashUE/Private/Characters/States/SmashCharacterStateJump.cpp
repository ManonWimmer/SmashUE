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
	float JumpVelocity = (2.0f * JumpMaxHeight) / JumpDuration;

	// Appliquer cette vélocité au personnage
	FVector JumpImpulse = FVector(0.0f, 0.0f, JumpVelocity);
	Character->LaunchCharacter(JumpImpulse, false, true);

	// Mémoriser la position de départ du saut
	JumpLocation = Character->GetActorLocation();
}


void USmashCharacterStateJump::CalculateJumpVelocity()
{
	FVector Velocity = Character->GetVelocity();
	float ZVelocity = Velocity.Z;
	
	// Calcul de la durée du saut et de la vélocité verticale en fonction de JumpMaxHeight

	float DesiredVelocityZ = (2.0f * JumpMaxHeight) / JumpDuration;

	// Appliquer la vélocité ajustée pour conserver la même durée de saut
	if (ZVelocity > 0 && Character->GetActorLocation().Z - JumpLocation.Z < JumpMaxHeight)
	{
		FVector NewVelocity = Velocity;
		NewVelocity.Z = FMath::Min(DesiredVelocityZ, ZVelocity); // Limiter la vitesse verticale
		Character->GetCharacterMovement()->Velocity = NewVelocity;
	}

	// Si on dépasse la hauteur maximale
	if (Character->GetActorLocation().Z - JumpLocation.Z > JumpMaxHeight)
	{
		// Stopper la montée et passer à l'état de chute
		FVector NewVelocity = Velocity;
		NewVelocity.Z = 0.0f;
		Character->GetCharacterMovement()->Velocity = NewVelocity;
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

