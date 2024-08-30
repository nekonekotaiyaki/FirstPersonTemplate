// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"

ADefaultHUD::ADefaultHUD()
{
	mTextScale = 1.0f;
	mIsVisible = false;
}

void ADefaultHUD::SetSwitchingTime(const FString& text, const FVector2D& pos, float scale)
{
	mText = text;
	mTextPos = pos;
	mTextScale = (scale <= 0.0f) ? 1.0f : scale;
}

void ADefaultHUD::SetVisible(bool f)
{
	mIsVisible = f;
}

void ADefaultHUD::DrawHUD()
{
	Super::DrawHUD();
	
	if (mIsVisible) {
		DrawText(
			mText,
			FLinearColor::White,
			mTextPos.X,
			mTextPos.Y,
			nullptr,
			mTextScale);
	}
}