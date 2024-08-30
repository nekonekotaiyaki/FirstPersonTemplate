// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonGameMode.h"
#include "FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "DefaultHUD.h"
#include "TimerManager.h"

#define	SPAN	(1.0f)
#define	SCALE	(3.0f)	// テキストスケール 

AFirstPersonGameMode::AFirstPersonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// HUDをもらっておく 
	HUDClass = ADefaultHUD::StaticClass();

	mRemainingTime = 0.0f;
	mSwitchingPeriod = 0.0f;
	mIsRunning = false;
}


void AFirstPersonGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* p = GetWorld()->GetFirstPlayerController();
	if (p) {
		mHUD = p->GetHUD();
	}
}

void AFirstPersonGameMode::SetTextLocation(const FVector2D &loc)
{
	mTextLocation = loc;
}

void AFirstPersonGameMode::StartTimer(float period, FOnTimeAttackStartDelegate delegate)
{
	if (!mIsRunning && mHUD) {
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			this,
			&AFirstPersonGameMode::OnTimerUpdate,
			SPAN,
			true);
		mRemainingTime = period;
		mSwitchingPeriod = period;
		mOnTimeAttackStart = delegate;
		mIsRunning = true;

		ADefaultHUD *hud = Cast<ADefaultHUD>(mHUD);
		if (hud) {
			hud->SetVisible(true);
			hud->SetSwitchingTime(
				FString::FromInt((int32)mRemainingTime),
				mTextLocation,
				SCALE);
		}
	}
}
void AFirstPersonGameMode::ResetTimer()
{
	if (mIsRunning && mHUD) {
		GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
		mIsRunning = false;

		ADefaultHUD *hud = Cast<ADefaultHUD>(mHUD);
		if (hud) {
			hud->SetVisible(false);
		}
	}
}

float AFirstPersonGameMode::SwitchingPeriod()
{
	return (mSwitchingPeriod);
}


void AFirstPersonGameMode::OnTimerUpdate()
{
	mRemainingTime -= SPAN;
	if (mRemainingTime <= 0.0f) {
		if (mOnTimeAttackStart.IsBound()) {
			mOnTimeAttackStart.Execute();
		}
		ResetTimer();
	}

	ADefaultHUD *hud = Cast<ADefaultHUD>(mHUD);
	if (hud) {
		hud->SetSwitchingTime(
			FString::FromInt((int32)mRemainingTime),
			mTextLocation,
			SCALE);
	}

}
