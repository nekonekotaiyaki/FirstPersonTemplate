// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#define	HIGH_SCORE_DEFAULT	(100)

ADefaultHUD::ADefaultHUD()
{
	mSwitchingCountTextScale = 1.0f;
	mIsSwitchingCountVisible = false;

	// ウィジェットブループリント 
	mUIHighScore = nullptr;
	mUIScore = nullptr;
	mUIPowerGauge = nullptr;

}

void ADefaultHUD::BeginPlay()
{
	Super::BeginPlay();

	// ウィジェットクラスを同期的にロード
	FString defaultWidgetPath = TEXT("/Game/FirstPerson/Blueprints/BP_DefaultWidget.BP_DefaultWidget_C");
	TSubclassOf<UUserWidget> defaultWidget = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*defaultWidgetPath)).LoadSynchronous();
	if (defaultWidget) {
		UUserWidget *p = CreateWidget<UUserWidget>(GetWorld(), defaultWidget);
		if (p) {
			p->AddToViewport();

			// 取り出しておく 
			mUIHighScore = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("HighScore")));
			mUIScore = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("Score")));
			mUIPowerGauge = Cast<UProgressBar>(p->GetWidgetFromName(TEXT("PowerGauge")));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("ADefaultHUD::BeginPlay!"));
}


void ADefaultHUD::DrawHUD()
{
	Super::DrawHUD();
	
	if (mIsSwitchingCountVisible) {
		DrawText(
			mSwitchingCountText,
			FLinearColor::White,
			mSwitchingCountTextPos.X,
			mSwitchingCountTextPos.Y,
			nullptr,
			mSwitchingCountTextScale);
	}
}

void ADefaultHUD::SetSwitchingTime(const FString &text, const FVector2D &pos, float scale)
{
	mSwitchingCountText = text;
	mSwitchingCountTextPos = pos;
	mSwitchingCountTextScale = (scale <= 0.0f) ? 1.0f : scale;
}

void ADefaultHUD::SetSwitchingTimeVisible(bool f)
{
	mIsSwitchingCountVisible = f;
}

void ADefaultHUD::SetHigh(int high)
{
	if (mUIHighScore) {
		FString text = FString::Printf(TEXT("%03d"), high);
		mUIHighScore->SetText(FText::FromString(text));
	}
}

void ADefaultHUD::SetScore(int score)
{
	if (mUIScore) {
		FString text = FString::Printf(TEXT("%03d"), score);
		mUIScore->SetText(FText::FromString(text));
	}
}
void ADefaultHUD::SetPower(float percent)
{
	if (mUIPowerGauge) {
		mUIPowerGauge->SetPercent(percent);
	}
}

int ADefaultHUD::GetDefaultHighScore()
{
	return (HIGH_SCORE_DEFAULT);
}

