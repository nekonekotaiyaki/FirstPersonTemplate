// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TimeAttackHUD.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSON_API ATimeAttackHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATimeAttackHUD();

	void BeginPlay() override;

	void SetRestTime(int sec, int msec);
	void SetScore(int score);
	void SetHigh(int high);
	void SetPower(float percent);


private:
	
	// ウィジェットブループリント 
	class UTextBlock *mUITimerMsec;
	class UTextBlock *mUITimerSec;
	class UTextBlock *mUIHighScore;
	class UTextBlock *mUIScore;
	class UProgressBar *mUIPowerGauge;

};
