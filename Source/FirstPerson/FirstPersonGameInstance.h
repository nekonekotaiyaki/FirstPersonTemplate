// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FirstPersonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSON_API UFirstPersonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFirstPersonGameInstance();

	void SetScore(int score);
	void SetHighScore(int high);

	int Score();
	int High();

private:
	int	mHighScore;
	int	mScore;

};
