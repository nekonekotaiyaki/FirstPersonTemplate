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

	// HUD��������Ă��� 
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

	// �\�������� 
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

	// HUD�ɔ��f 
	ATimeAttackHUD *hud = Cast<ATimeAttackHUD>(mHUD);
	if (hud) {
		hud->SetRestTime(GetSec(), GetMsec());
	}

	// �I���`�F�b�N 
	if (mRemainingTime <= 0.0f) {
		GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("TimeAttackEnd"));
	}
}
