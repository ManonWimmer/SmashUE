// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterState.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State %d"), GetStateID())
		);

	CharacterSettings = GetDefault<USmashCharacterSettings>();
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousStateID)
{
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID)
{
}

void USmashCharacterState::StateTick(float DeltaTime)
{
}


