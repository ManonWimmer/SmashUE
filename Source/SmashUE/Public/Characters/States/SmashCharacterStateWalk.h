// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateWalk.generated.h"

class UCharacterMovementComponent;

UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateWalk : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	UFUNCTION()
	void OnInputJump(float InputJumpValue);
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;
	
	UFUNCTION()
	void OnInputMoveXFast(float InputMoveX);
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* WalkAnim;

	UPROPERTY(EditAnywhere)
	float WalkMoveSpeedMax = 250.f;
};
