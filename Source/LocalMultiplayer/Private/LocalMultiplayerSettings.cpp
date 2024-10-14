// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"
#include "InputMappingContext.h"


// Retourne vraie si l’input FKey fournie est contenu dans l’IMC correspondant au MappingType fourni.
bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	bool TempContains = false;
	UInputMappingContext* IMC = GetIMCFromType(MappingType);
	for (FEnhancedActionKeyMapping IMCKey : IMC->GetMappings())
	{
		if (Key == IMCKey.Key) TempContains = true;
	}

	return TempContains;
}

// Retourne l’IMC correspondant au MappingType fourni.
UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType) {
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	}
	return nullptr;
}

// Retourne la taille du nombre de profil claviers disponibles dans les settings.
int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

// Retourne l’index d’un profil clavier en fonction de la touche saisie et du profil fourni (-1 si aucune touche n’a été trouvé).
int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); i++)
	{
		if (KeyboardProfilesData[i].ContainsKey(Key, MappingType)) return i;
	}
	
	return -1;
}
