// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TimeAttackGameMode.generated.h"


/**
 * 
 */
UCLASS()
class FIRSTPERSON_API ATimeAttackGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATimeAttackGameMode();

	virtual void BeginPlay() override;

private:
	int GetSec();
	int GetMsec();
	void OnTimerUpdate();

	AHUD	*mHUD;

	float	mTimeAttackPeriod;
	float	mRemainingTime;
	FTimerHandle	mTimerHandle;

};
