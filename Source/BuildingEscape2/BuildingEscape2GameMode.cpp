// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BuildingEscape2.h"
#include "BuildingEscape2GameMode.h"
#include "BuildingEscape2HUD.h"
#include "BuildingEscape2Character.h"

ABuildingEscape2GameMode::ABuildingEscape2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABuildingEscape2HUD::StaticClass();
}
