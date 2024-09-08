// Copyright Epic Games, Inc. All Rights Reserved.


#include "FirstPersonPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TimeAttackGameMode.h"


AFirstPersonPlayerController::AFirstPersonPlayerController()
{
	mIsTimeAttackMode = false;
}
bool AFirstPersonPlayerController::IsTimeAttackMode()
{
	return (mIsTimeAttackMode);
}

void AFirstPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("AddMapiingContext!"));
	}

	// ƒQ[ƒ€ƒ‚[ƒhŽæ“¾ 
	AGameModeBase *gm = GetWorld()->GetAuthGameMode();
	mIsTimeAttackMode = (gm && Cast<ATimeAttackGameMode>(gm));
}

