// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "FirstPersonGameInstance.h"

#define	HIGH_SCORE_DEFAULT	(20)
#define	UPDATE_SPEED		(1.0f / 60.0f)

ADefaultHUD::ADefaultHUD()
{
	mSwitchingCountTextScale = 1.0f;
	mIsSwitchingCountVisible = false;

	// ウィジェットブループリント 
	mUITimerSec = nullptr;
	mUITimerMsec = nullptr;
	mUIHighScore = nullptr;
	mUIScore = nullptr;
	mUIPowerGauge = nullptr;
	mUIFadeImage = nullptr;


	mFadeSpeed = 0.0f;
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
			mUITimerSec = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("TimerSec")));
			mUITimerMsec = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("TimerMsec")));
			mUIHighScore = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("HighScore")));
			mUIScore = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("Score")));
			mUIPowerGauge = Cast<UProgressBar>(p->GetWidgetFromName(TEXT("PowerGauge")));
			mUIFadeImage = Cast<UImage>(p->GetWidgetFromName(TEXT("FadeImage")));
		}
	}

	// 非表示 
	if (mUIPowerGauge) {
		mUIPowerGauge->SetVisibility(ESlateVisibility::Hidden);
	}
	if (mUITimerSec) {
		mUITimerSec->SetVisibility(ESlateVisibility::Hidden);
	}
	if (mUITimerMsec) {
		mUITimerMsec->SetVisibility(ESlateVisibility::Hidden);
	}
	if (mUIFadeImage) {
		mUIFadeImage->SetVisibility(ESlateVisibility::Hidden);
	}


	// 表示初期化 
	UFirstPersonGameInstance *gameInst = Cast<UFirstPersonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInst) {
		if (gameInst->High() < HIGH_SCORE_DEFAULT) {
			gameInst->SetHighScore(HIGH_SCORE_DEFAULT);
		}
		SetScore(gameInst->Score());
		SetHigh(gameInst->High());
	}
	//UE_LOG(LogTemp, Warning, TEXT("ADefaultHUD::BeginPlay!"));
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

	// ゲージが出ているときはレティクルを表示する 
	if (mUIPowerGauge && mUIPowerGauge->IsVisible()) {
		FVector2D center = FVector2D(Canvas->SizeX, Canvas->SizeY) * 0.5f;
		DrawText(
			TEXT("+"),
			FLinearColor::White,
			center.X,
			center.Y,
			nullptr,
			2.0f);
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
void ADefaultHUD::DisponGauge()
{
	if (mUIPowerGauge) {
		mUIPowerGauge->SetVisibility(ESlateVisibility::Visible);
	}
}

int ADefaultHUD::GetDefaultHighScore()
{
	return (HIGH_SCORE_DEFAULT);
}

void ADefaultHUD::StartFadeOut(float duration, FOnFadeOutDelegate delegate)
{
	if (mUIFadeImage) {
		mUIFadeImage->SetVisibility(ESlateVisibility::Visible);
		mFadeSpeed = 1.0f / duration;
		mOnFadeOut = delegate;

		// タイマーで色を変化させる（フェードアウト処理）
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			this,
			&ADefaultHUD::OnUpdateFade,
			UPDATE_SPEED,
			true);
	}
}

void ADefaultHUD::OnUpdateFade()
{
	if (mUIFadeImage) {
		FLinearColor color = mUIFadeImage->ColorAndOpacity;
		color.A += (mFadeSpeed * UPDATE_SPEED);
		color.A = FMath::Clamp(color.A, 0.0f, 1.0f);
		mUIFadeImage->SetColorAndOpacity(color);
		if (color.A == 1.0f) {
			GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
			if (mOnFadeOut.IsBound()) {
				mOnFadeOut.Execute();
			}
		}
	}
}

