// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealAOEDamageGameMode.h"
#include "UnrealAOEDamageCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealAOEDamageGameMode::AUnrealAOEDamageGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
