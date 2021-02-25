// Copyright Epic Games, Inc. All Rights Reserved.


#include "CDGameModeBase.h"
#include "Player/CDPlayerController.h"
#include "Core/CDCarBase.h"

ACDGameModeBase::ACDGameModeBase() {
	DefaultPawnClass = ACDCarBase::StaticClass();
	PlayerControllerClass = ACDPlayerController::StaticClass();
}