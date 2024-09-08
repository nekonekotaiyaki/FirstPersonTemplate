// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TimeAttackGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSON_API ATimeAttackGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATimeAttackGameMode();

	virtual void BeginPlay() override;


	FName ScoringTagName();

	void SetOnGetScoreCallback(AActor *actor);

private:
	int GetSec();
	int GetMsec();
	void OnTimerUpdate();

	UFUNCTION()
	void OnGetScore();

	UFUNCTION()
	void OnEndFadeOut();

	AHUD	*mHUD;

	float	mRemainingTime;
	FTimerHandle	mTimerHandle;

	FName	mScoringTagName;
};
