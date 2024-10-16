// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/SmashCharacterInputData.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "EnhancedInputComponent.h"
#include "Characters/SmashCharacterStateID.h"
#include "Components/CapsuleComponent.h"
#include "Components/DynamicMeshComponent.h"


// Sets default values	
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASmashCharacter::OnCollisionEnter);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ASmashCharacter::OnCollisionExit);
	}
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();

	CreateStateMachine();
	InitStateMachine();
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
	BindInputJumpAndActions(EnhancedInputComponent);
	BindInputMoveYAxisAndActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;

	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	//InputSystem->AddMappingContext(InputMappingContext, 0);
}

#pragma region Input Move X

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveXValue;
}

void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveX);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX);
	}

	if (InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveXFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveXFast);
	}
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveXValue = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveXValue = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveXValue);
}

#pragma endregion

#pragma region Input Jump

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputJumpEvent.Broadcast(InputActionValue.Get<float>());
}

void ASmashCharacter::BindInputJumpAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputJump);
	}
}

#pragma endregion

#pragma region Input Move Y

float ASmashCharacter::GetInputMoveYValue() const
{
	return InputMoveYValue;
}

void ASmashCharacter::BindInputMoveYAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionMoveY)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveY);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveY);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveY);
	}

	if (InputData->InputActionMoveYFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveYFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveYFast);
	}
}

void ASmashCharacter::OnInputMoveY(const FInputActionValue& InputActionValue)
{
	InputMoveYValue = InputActionValue.Get<float>();
	InputMoveYEvent.Broadcast(InputMoveYValue);
}

void ASmashCharacter::OnInputMoveYFast(const FInputActionValue& InputActionValue)
{
	InputMoveYValue = InputActionValue.Get<float>();
	InputMoveYFastEvent.Broadcast(InputMoveYValue);
	CheckForOneWayPlatform();
}

void ASmashCharacter::CheckForOneWayPlatform()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 100.f);
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility
	);

	if (HitResult.bBlockingHit && HitResult.GetActor()->Tags.Contains("OneWayPlatform"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("One way platform"));

		GetCapsuleComponent()->IgnoreComponentWhenMoving(
			HitResult.GetActor()->FindComponentByClass<UDynamicMeshComponent>(), true);

		if (HitResult.GetActor()->FindComponentByClass<UDynamicMeshComponent>() == nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("pas trouvé static mesh"));
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("trouvé static mesh"));
	}
}


#pragma endregion

void ASmashCharacter::OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	if (OtherActor->Tags.Contains("OneWayPlatform"))
	{
		GetCapsuleComponent()->IgnoreComponentWhenMoving(
			OtherActor->FindComponentByClass<UDynamicMeshComponent>(), true);
	}
}

void ASmashCharacter::OnCollisionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;

	if (OtherActor->Tags.Contains("OneWayPlatform"))
	{
		GetCapsuleComponent()->IgnoreComponentWhenMoving(
			OtherActor->FindComponentByClass<UDynamicMeshComponent>(), false);
	}
}
