// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackHUD.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


ATimeAttackHUD::ATimeAttackHUD()
{
	mUITimerMsec = nullptr;
	mUITimerSec = nullptr;
	mUIHighScore = nullptr;
	mUIScore = nullptr;
	mUIPowerGauge = nullptr;
}

void ATimeAttackHUD::BeginPlay()
{
	Super::BeginPlay();

	// �E�B�W�F�b�g�N���X�𓯊��I�Ƀ��[�h
	FString timeAttackWidgetPath = TEXT("/Game/FirstPerson/Blueprints/BP_TimeAttackWidget.BP_TimeAttackWidget_C");
	TSubclassOf<UUserWidget> timeAttackWidget = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*timeAttackWidgetPath)).LoadSynchronous();
	if (timeAttackWidget) {
		UUserWidget *p = CreateWidget<UUserWidget>(GetWorld(), timeAttackWidget);
		if (p) {
			p->AddToViewport();

			// ���o���Ă��� 
			mUIHighScore = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("HighScore")));
			mUIScore = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("Score")));
			mUIPowerGauge = Cast<UProgressBar>(p->GetWidgetFromName(TEXT("PowerGauge")));
			mUITimerSec = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("TimerSec")));
			mUITimerMsec = Cast<UTextBlock>(p->GetWidgetFromName(TEXT("TimerMsec")));
		}
	}
}
void ATimeAttackHUD::DrawHUD()
{
}


void ATimeAttackHUD::SetRestTime(int sec, int msec)
{
}



