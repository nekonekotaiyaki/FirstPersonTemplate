// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "TimeAttackHUD.h"
#include "GameModeSwitchObject.h"
#include "FirstPersonGameInstance.h"
#include "FirstPersonPlayerController.h"
#include "FirstPersonProjectile.h"


#define	TIMEATTACK_PERIOD	(10.0f)
#define	SPAN				(0.01f)
#define	SCORING_TAG_NAME	("ScoringCube")
#define	FADE_DURATION		(1.0f)

ATimeAttackGameMode::ATimeAttackGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerControllerClass = AFirstPersonPlayerController::StaticClass();


	// HUDをもらっておく 
	HUDClass = ATimeAttackHUD::StaticClass();
	
	mRemainingTime = TIMEATTACK_PERIOD;
	mHUD = nullptr;
	
	// タグ名 
	mScoringTagName = FName(SCORING_TAG_NAME);
}
void ATimeAttackGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController *p = GetWorld()->GetFirstPlayerController();
	if (p) {
		mHUD = p->GetHUD();
	}

	TArray<AActor*> list;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), list);
	for (AActor *a : list) {
		AGameModeSwitchObject *parent = Cast<AGameModeSwitchObject>(a);
		if (parent) {
			parent->SetTimeAttackMode();
		}
		else {
			FString label = a->GetActorLabel();
			if (label.Contains("SM_ChamferCube")) {
				a->Tags.Add(mScoringTagName);
			}
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
	return (FMath::RoundToInt(part * 100.0f));
}
FName ATimeAttackGameMode::ScoringTagName()
{
	return (mScoringTagName);
}
void ATimeAttackGameMode::SetOnGetScoreCallback(AActor *actor)
{
	AFirstPersonProjectile *projectile = Cast<AFirstPersonProjectile>(actor);
	if (projectile) {
		FOnGetScoreDelegate delegate = FOnGetScoreDelegate::CreateUObject(this, &ATimeAttackGameMode::OnGetScore);
		projectile->SetOnGetScore(delegate);
		projectile->SetScoringTagName(mScoringTagName);
	}
}



void ATimeAttackGameMode::OnTimerUpdate()
{
	ATimeAttackHUD *hud = Cast<ATimeAttackHUD>(mHUD);
	mRemainingTime -= SPAN;

	// 終了チェック 
	if (mRemainingTime <= 0.0f) {
		mRemainingTime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("TimeAttackEnd"));

		if (hud) {
			FOnFadeOutDelegate delegate = FOnFadeOutDelegate::CreateUObject(this, &ATimeAttackGameMode::OnEndFadeOut);
			hud->StartFadeOut(FADE_DURATION, delegate);
		}
	}

	// HUDに反映 
	if (hud) {
		hud->SetRestTime(GetSec(), GetMsec());
	}
}
void ATimeAttackGameMode::OnGetScore()
{
	ATimeAttackHUD *hud = Cast<ATimeAttackHUD>(mHUD);
	UFirstPersonGameInstance *gameInst = Cast<UFirstPersonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (hud && gameInst) {
		int high = gameInst->High();
		int score = gameInst->Score();

		// スコア更新 
		score++;
		if (score > 999) {
			// 3桁まで 
			score = 999;
		}
		gameInst->SetScore(score);

		// ハイスコア更新 
		if (score > high) {
			high = score;
			gameInst->SetHighScore(high);
		}


		// 画面更新 
		hud->SetScore(score);
		hud->SetHigh(high);
	}
}
void ATimeAttackGameMode::OnEndFadeOut()
{
	FName mapName = TEXT("/Game/FirstPerson/Maps/FirstPersonMap");
	UGameplayStatics::OpenLevel(GetWorld(), mapName);
}
