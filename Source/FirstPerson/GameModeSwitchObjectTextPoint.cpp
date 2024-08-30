// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSwitchObjectTextPoint.h"

// Sets default values
AGameModeSwitchObjectTextPoint::AGameModeSwitchObjectTextPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネントの初期化
	mRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = mRootComp;
}

// Called when the game starts or when spawned
void AGameModeSwitchObjectTextPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameModeSwitchObjectTextPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

