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

	virtual void DrawHUD() override;

	void SetVisible(bool f);
	void SetSwitchingTime(const FString &text, const FVector2D &pos, float scale);

private:
	FString	mText;
	FVector2D	mTextPos;
	float	mTextScale;
	bool	mIsVisible;
};
