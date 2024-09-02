// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModeSwitcher.generated.h"


UCLASS()
class FIRSTPERSON_API AGameModeSwitcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameModeSwitcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDefaultGameMode(AGameModeBase *gm);
	void RestoreDefaultGameMode();
	AGameModeBase *SwitchToTimeAttack();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent	*mRootComp;

	UPROPERTY(EditAnywhere, Category = "Game Mode")
	TSubclassOf<AGameModeBase>	mTimeAttackGameModeClass;

	AGameModeBase	*mReturnTo;

};
