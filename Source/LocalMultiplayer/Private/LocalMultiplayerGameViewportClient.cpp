// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "LocalMultiplayer/LocalMultiplayerSubsystem.h"

// Passer le nombre de joueurs max de 4 à 8 (attribut MaxSplitscreenPlayers)
void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();

	MaxSplitscreenPlayers = 8;
}

// Détection des inputs + attribution des players index en fonction de la touche du clavier / bouton de la manette fourni en paramètre.
bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	if (!EventArgs.IsGamepad())
	{
		int KeyboardIndex =
			Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboardIndex >= 0)
		{
			int PlayerIndex = Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardIndex);
		
			if (PlayerIndex < 0)
			{
				PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardIndex);

				Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardIndex,
											 ELocalMultiplayerInputMappingType::InGame);

				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
				const FInputKeyParams KeyParams(EventArgs.Key, EventArgs.Event, 0);
				PlayerController->InputKey(KeyParams);
			}
			else return Super::InputKey(EventArgs);
		}
		else return Super::InputKey(EventArgs);
	}
	else
	{
		int PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId());

		if (PlayerIndex == -1)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.InputDevice.GetId());

			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
			const FInputKeyParams KeyParams(EventArgs.Key, EventArgs.Event, 0);
			PlayerController->InputKey(KeyParams);
		}
		else return Super::InputKey(EventArgs);
	}

	return true;
}

// Détection des inputs + attributions des player index en fonction des axes (joysticks à la manette) fourni en paramètre.
bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
                                                    float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	

	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	if (!bGamepad)
	{
		int KeyboardIndex =
			Settings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboardIndex >= 0)
		{
			int PlayerIndex = Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardIndex);
		
			if (PlayerIndex < 0)
			{
				PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardIndex);

				Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardIndex,
											 ELocalMultiplayerInputMappingType::InGame);

				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
				FInputKeyParams KeyParams(Key, Delta, DeltaTime, NumSamples, bGamepad, InputDevice);
				PlayerController->InputKey(KeyParams);
			}
			else return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
		else return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
	else
	{
		int PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());

		if (PlayerIndex == -1)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());

			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
			FInputKeyParams KeyParams(Key, Delta, DeltaTime, NumSamples, bGamepad, InputDevice);
			PlayerController->InputKey(KeyParams);
		}
		else return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	return true;
}
