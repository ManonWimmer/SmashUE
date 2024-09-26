// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;
	
	UFUNCTION()
	void OnInputJump();
	void CalculateJumpVelocity();

	UPROPERTY(EditAnywhere)
	UAnimMontage* JumpAnim;

	UPROPERTY(EditAnywhere)
	float JumpMaxHeight = 200.f;

	UPROPERTY(EditAnywhere)
	float JumpDuration = 3.f;

private:
	FVector JumpLocation;
};
