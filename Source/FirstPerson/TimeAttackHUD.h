// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultHUD.h"
#include "TimeAttackHUD.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSON_API ATimeAttackHUD : public ADefaultHUD
{
	GENERATED_BODY()

public:
	ATimeAttackHUD();
	void BeginPlay() override;
	virtual void DrawHUD() override;

	void SetRestTime(int sec, int msec);

};
