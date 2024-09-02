// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "TimeAttackHUD.h"
#include "FirstPersonGameInstance.h"
#include "Math/UnrealMathUtility.h"

#define	TIMEATTACK_PERIOD	(10.0f)
#define	SPAN				(0.001f)

ATimeAttackGameMode::ATimeAttackGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// HUDをもらっておく 
	HUDClass = ATimeAttackHUD::StaticClass();
	
	mTimeAttackPeriod = TIMEATTACK_PERIOD;
	mRemainingTime = 0.0f;
	mHUD = nullptr;
}
void ATimeAttackGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController *p = GetWorld()->GetFirstPlayerController();
	if (p) {
		mHUD = p->GetHUD();
	}

	// 表示初期化 
	UFirstPersonGameInstance *gameInst = Cast<UFirstPersonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInst) {
		ATimeAttackHUD *hud = Cast<ATimeAttackHUD>(mHUD);
		if (hud) {
			hud->SetScore(gameInst->Score());
			hud->SetHigh(gameInst->High());
			hud->SetRestTime(GetSec(), GetMsec());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		mTimerHandle,
		this,
		&ATimeAttackGameMode::OnTimerUpdate,
		SPAN,
		true);


	UE_LOG(LogTemp, Warning, TEXT("ATimeAttackGameMode::BeginPlay!"));
}

int ATimeAttackGameMode::GetSec()
{
	return (FMath::FloorToInt(mRemainingTime));
}
int ATimeAttackGameMode::GetMsec()
{
	float part = mRemainingTime - (float)GetSec();
	return (FMath::RoundToInt(part * 1000.0f));
}

void ATimeAttackGameMode::OnTimerUpdate()
{
	mRemainingTime -= SPAN;

	// HUDに反映 
	ATimeAttackHUD *hud = Cast<ATimeAttackHUD>(mHUD);
	if (hud) {
		hud->SetRestTime(GetSec(), GetMsec());
	}

	// 終了チェック 
	if (mRemainingTime <= 0.0f) {
		GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("TimeAttackEnd"));
	}
}
