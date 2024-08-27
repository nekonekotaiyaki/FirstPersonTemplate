// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSwitchTrigger.h"

// Sets default values
AGameModeSwitchTrigger::AGameModeSwitchTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameModeSwitchTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameModeSwitchTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

