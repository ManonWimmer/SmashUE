// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

// Création des PlayerControllers en fonction du nombre de settings clavier / manettes disponibles dans les paramètres.
void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "CreateAndInitPlayers");
	
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	for (int i = 0; i < Settings->GetNbKeyboardProfiles(); i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i, true);
	}

	for (int i = 0; i < Settings->NbMaxGamepads; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i+Settings->GetNbKeyboardProfiles(), true);
	}

	// boucle tmap
}

// Retrouve l’index d’un PlayerController à partir de l’index d’un profil clavier (retourne -1 s’il n’existe pas encore de mapping).
int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if (!PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex)) return -1;

	return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
}

// Assigne un nouvel index d’un PlayerController pour l’index du profil clavier fourni (retourne le joueur qui vient d’être assigné).
int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "AssignNewPlayerToKeyboardProfile");
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

// Associe l’IMC correspondant à l’index du profil clavier fourni à l’index du Player Controller fourni.
void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, "AssignKeyboardMapping");
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	
	UInputMappingContext* IMC = Settings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	FModifyContextOptions ContextOptions;
	ContextOptions.bForceImmediately = true;
	PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, 1, ContextOptions);
}

// Retrouve l’index d’un PlayerController à partir de l’index du device fourni (retourne -1 s’il n’existe pas encore de mapping).
int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if (!PlayerIndexFromGamepadProfileIndex.Contains(DeviceID)) return -1;

	return PlayerIndexFromGamepadProfileIndex[DeviceID];
}

// Assigne un nouvel index d’un PlayerController pour l’index du device fourni (retourne le joueur qui vient d’être assigné).
int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "AssignNewPlayerToGamepadDeviceID");
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

// Associe l’IMC du Gamepad à l’index du Player Controller fourni.
void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "AssignGamepadInputMapping");
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	
	UInputMappingContext* IMC = Settings->GamepadProfileData.GetIMCFromType(MappingType);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	FModifyContextOptions ContextOptions;
	ContextOptions.bForceImmediately = true;
	PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, 1, ContextOptions);
}
