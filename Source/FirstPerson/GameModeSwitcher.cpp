// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSwitcher.h"
#include "TimeAttackGameMode.h"

// Sets default values
AGameModeSwitcher::AGameModeSwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポネントの追加 
	mRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = mRootComp;

	mReturnTo = nullptr;
}

// Called when the game starts or when spawned
void AGameModeSwitcher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameModeSwitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameModeSwitcher::SetDefaultGameMode(AGameModeBase *gm)
{
	mReturnTo = gm;
}
void AGameModeSwitcher::RestoreDefaultGameMode()
{
	if (mReturnTo) {

	}
}
AGameModeBase *AGameModeSwitcher::SwitchToTimeAttack()
{
	if (mReturnTo) {
		AGameModeBase *gm = GetWorld()->SpawnActor<ATimeAttackGameMode>(mTimeAttackGameModeClass);
		return (gm);
	}
	return (nullptr);
}


