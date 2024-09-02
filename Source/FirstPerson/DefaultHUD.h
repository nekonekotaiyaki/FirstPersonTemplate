// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DefaultHUD.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSON_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ADefaultHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	void SetSwitchingTimeVisible(bool f);
	void SetSwitchingTime(const FString &text, const FVector2D &pos, float scale);

	void SetHigh(int high);
	void SetScore(int score);
	void SetPower(float percent);

	int GetDefaultHighScore();

private:

	// スイッチカウント 
	FString	mSwitchingCountText;
	FVector2D	mSwitchingCountTextPos;
	float	mSwitchingCountTextScale;
	bool	mIsSwitchingCountVisible;

protected:
	
	// ウィジェットブループリント 
	class UTextBlock	*mUIHighScore;
	class UTextBlock	*mUIScore;
	class UProgressBar	*mUIPowerGauge;

};
