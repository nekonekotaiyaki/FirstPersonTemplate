// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModeSwitchObject.generated.h"

UCLASS()
class FIRSTPERSON_API AGameModeSwitchObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameModeSwitchObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor *GetTextPointComp();

	void StartSwitching(float period);
	void StopSwitching();
	void SetTimeAttackMode();

private:
	bool ChangeColorToTimeAttack();
	bool ChangeColorToDefault(float period);


    // メッシュ 
    UPROPERTY(VisibleAnywhere, Category = "StaticMesh")
    UStaticMeshComponent *mMeshComponent;

	// カラーコントロール 
	UMaterialInstanceDynamic	*mMaterialHandle;
	float	mPeriod;
	float	mTimer;
	int	mFlags;

	// モード 
	bool	mIsTimeAttackMode;
};
