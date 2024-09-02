// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModeSwitchTrigger.generated.h"

UCLASS()
class FIRSTPERSON_API AGameModeSwitchTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameModeSwitchTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	class UBoxComponent* mTriggerBox;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent *overlappedComp,
		AActor *otherActor,
		UPrimitiveComponent *otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult &sweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent *overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent *otherComp,
		int32 otherBodyIndex);

private:
	UFUNCTION()
	void OnStartTimeAttack();

	UFUNCTION()
	void OnEndTimeAttack();

	AActor	*mParentObject;
	AActor	*mTextPoint;

	AGameModeBase	*mDefaultGameMode;
	APlayerController	*mPC;

};
