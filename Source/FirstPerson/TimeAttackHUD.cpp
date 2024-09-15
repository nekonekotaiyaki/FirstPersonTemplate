// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackHUD.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


ATimeAttackHUD::ATimeAttackHUD()
	: Super()
{
}

void ATimeAttackHUD::BeginPlay()
{
	Super::BeginPlay();

	// •\Ž¦ 
	if (mUITimerSec) {
		mUITimerSec->SetVisibility(ESlateVisibility::Visible);
	}
	if (mUITimerMsec) {
		mUITimerMsec->SetVisibility(ESlateVisibility::Visible);
	}
	//UE_LOG(LogTemp, Warning, TEXT("ATimeAttackHUD::BeginPlay!"));
}
void ATimeAttackHUD::DrawHUD()
{
	Super::DrawHUD();
}


void ATimeAttackHUD::SetRestTime(int sec, int msec)
{
	if (mUITimerSec) {
		FString text = FString::Printf(TEXT("%02d"), sec);
		mUITimerSec->SetText(FText::FromString(text));
	}
	if (mUITimerMsec) {
		FString text = FString::Printf(TEXT("%02d"), msec);
		mUITimerMsec->SetText(FText::FromString(text));
	}
}


