// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonGameInstance.h"


UFirstPersonGameInstance::UFirstPersonGameInstance()
{
	mHighScore = 0;
	mScore = 0;
}


void UFirstPersonGameInstance::SetScore(int score)
{
	mScore = score;
}
void UFirstPersonGameInstance::SetHighScore(int high)
{
	mHighScore = high;
}

int UFirstPersonGameInstance::Score()
{
	return (mScore);
}
int UFirstPersonGameInstance::High()
{
	return (mHighScore);
}
