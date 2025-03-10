// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSwitchObject.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameModeSwitchObjectTextPoint.h"

// フラグ 
#define	FLAG_TIMEATTACK			(1 << 31)
#define	FLAG_SWITCHING_GREEN	(1 << 1)
#define	FLAG_SWITCHING_RED		(1 << 0)

// Sets default values
AGameModeSwitchObject::AGameModeSwitchObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートメッシュを設定する 
	mMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(_TEXT("MeshComponent"));
	RootComponent = mMeshComponent;

	mMaterialHandle = nullptr;
	mFlags = 0;
	mTimer = 0.0f;
	mIsTimeAttackMode = false;
}

// Called when the game starts or when spawned
void AGameModeSwitchObject::BeginPlay()
{
	Super::BeginPlay();

	if (mMeshComponent) {
		UMaterialInterface *mat = mMeshComponent->GetMaterial(0);
		if (mat) {
			UMaterial *m = Cast<UMaterial>(mat);
			if (m) {
				mMaterialHandle = UMaterialInstanceDynamic::Create(m, this);
				if (mMaterialHandle) {
					mMeshComponent->SetMaterial(0, mMaterialHandle);
					mMaterialHandle->SetVectorParameterValue(FName("EmissiveColor"), FLinearColor::Green);
				}
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay!!!!!!"));
}

// Called every frame
void AGameModeSwitchObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// タイムアタック中は何もしない 
	if (mIsTimeAttackMode) {
		return;
	}

	if ((mFlags & FLAG_TIMEATTACK) == 0) {	
		if (mFlags & FLAG_SWITCHING_RED) {
			bool result = ChangeColorToTimeAttack();
			if (result) {
				mFlags &= ~FLAG_SWITCHING_RED;
				mFlags |= FLAG_TIMEATTACK;
			}
			else {
				mTimer += DeltaTime;
			}
		}
		else if (mFlags & FLAG_SWITCHING_GREEN) {
			bool result = ChangeColorToDefault();
			if (result) {
				mFlags &= ~FLAG_SWITCHING_GREEN;
				mTimer = 0.0f;
			}
			else {
				mTimer -= DeltaTime;
			}
		}
	}
}

// テキストポイントコンポネントを返す 
AActor *AGameModeSwitchObject::GetTextPointComp()
{
	TArray<AActor*> list;
	GetAttachedActors(list);

	for (AActor *child : list) {
		if (child) {
			AGameModeSwitchObjectTextPoint *target = Cast<AGameModeSwitchObjectTextPoint>(child);
			if (target) {
				return (target);
			}
		}
	}
	return (nullptr);
}

// タイムアタックゲームモードかどうか 
void AGameModeSwitchObject::SetTimeAttackMode()
{
	mIsTimeAttackMode = true;
	if (mMaterialHandle) {
		mMaterialHandle->SetVectorParameterValue(FName("EmissiveColor"), FLinearColor::Red);
	}
}



// スイッチ開始 
void AGameModeSwitchObject::StartSwitching(float period)
{
	if ((mFlags & FLAG_SWITCHING_RED) == 0) {
		mFlags |= FLAG_SWITCHING_RED;
		mPeriod = (period <= 0.0f) ? 1.0f : period;
		mTimer = 0.0f;
	}
}

// スイッチやめ 
void AGameModeSwitchObject::StopSwitching()
{
	if (mFlags & FLAG_SWITCHING_RED) {
		mFlags &= ~FLAG_SWITCHING_RED;
		mFlags |= FLAG_SWITCHING_GREEN;
		mTimer = mTimer / mPeriod;
	}
}

// タイムアタック用の色へ変更 
bool AGameModeSwitchObject::ChangeColorToTimeAttack()
{
	if (mMaterialHandle) {
		float scalar = FMath::Clamp(mTimer / mPeriod, 0.0f, 1.0f);
		FLinearColor target = FLinearColor::Red;
		FLinearColor start = FLinearColor::Green;
		FLinearColor current = FMath::Lerp(start, target, scalar);
		mMaterialHandle->SetVectorParameterValue(FName("EmissiveColor"), current);
		return (scalar >= 1.0f);
	}
	return (false);
}

// 色をデフォルトの戻す 
bool AGameModeSwitchObject::ChangeColorToDefault()
{
	if (mMaterialHandle) {
		float scalar = FMath::Clamp(mTimer / 1.0f, 0.0f, 1.0f);
		FLinearColor target = FLinearColor::Red;
		FLinearColor start = FLinearColor::Green;
		FLinearColor current = FMath::Lerp(start, target, scalar);
		mMaterialHandle->SetVectorParameterValue(FName("EmissiveColor"), current);
		//UE_LOG(LogTemp, Warning, TEXT("scalar=%f"), scalar);
		return (scalar <= 0.0f);
	}
	return (false);
}


