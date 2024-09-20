// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Unreal Default

public:
	UFUNCTION()
	void OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Orient

public:
	float GetOrientX() const;
	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadWrite)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;

#pragma endregion

#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;
#pragma endregion

#pragma region Input Move X
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXFastEvent, float, InputMoveX);

public:
	float GetInputMoveX() const;

	UPROPERTY()
	FInputMoveXFastEvent InputMoveXFastEvent;

protected:
	UPROPERTY()
	float InputMoveXValue = 0.f;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);

	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region Input Jump
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, float, InputJumpValue);

public:
	UPROPERTY()
	FInputJumpEvent InputJumpEvent;

private:
	void OnInputJump(const FInputActionValue& InputActionValue);
	void BindInputJumpAndActions(UEnhancedInputComponent* EnhancedInputComponent);


#pragma endregion

#pragma region Input Crouch
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveYEvent, float, InputMoveYValue);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveYFastEvent, float, InputMoveYValue);

public:
	UPROPERTY()
	FInputMoveYEvent InputMoveYEvent;

	UPROPERTY()
	FInputMoveYEvent InputMoveYFastEvent;

	float GetInputMoveYValue() const;

protected:
	float InputMoveYValue = 0.f;

private:
	void BindInputMoveYAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveY(const FInputActionValue& InputActionValue);

	void OnInputMoveYFast(const FInputActionValue& InputActionValue);

	void CheckForOneWayPlatform();

#pragma endregion
};
