// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstPersonGameMode.generated.h"


DECLARE_DELEGATE(FOnTimeAttackStartDelegate);

UCLASS(minimalapi)
class AFirstPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFirstPersonGameMode();

	virtual void BeginPlay() override;

	void SetTextLocation(const FVector2D &loc);
	void StartTimer(float period, FOnTimeAttackStartDelegate delegate);
	void ResetTimer();
	float SwitchingPeriod();

	void GotoTimeAttackGameMode();

private:
	void OnTimerUpdate();

	UFUNCTION()
	void OnEndFadeOut();

	class AHUD *mHUD;

	FVector2D	mTextLocation;

	bool	mIsRunning;
	float	mSwitchingPeriod;
	float	mRemainingTime;
	FTimerHandle	mTimerHandle;
	FOnTimeAttackStartDelegate	mOnTimeAttackStart;
};



