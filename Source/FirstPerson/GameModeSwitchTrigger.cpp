// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSwitchTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonGameMode.h"
#include "DefaultHUD.h"
#include "GameModeSwitchObject.h"
#include "GameModeSwitchObjectTextPoint.h"

// Sets default values
AGameModeSwitchTrigger::AGameModeSwitchTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// UBoxComponent �̍쐬
	mTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = mTriggerBox;

	if (mTriggerBox) {
		mTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGameModeSwitchTrigger::OnOverlapBegin);
		mTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGameModeSwitchTrigger::OnOverlapEnd);
		mTriggerBox->SetGenerateOverlapEvents(true);
		mTriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		mTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		mTriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	}


	mParentObject = nullptr;
	mTextPoint = nullptr;
}

// Called when the game starts or when spawned
void AGameModeSwitchTrigger::BeginPlay()
{
	Super::BeginPlay();

	// �e���� 
	TArray<AActor*> list;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameModeSwitchObject::StaticClass(), list);

	// �e�L�X�g�|�C���g�擾 
	for (AActor *a : list) {
		AGameModeSwitchObject *parent = Cast<AGameModeSwitchObject>(a);
		if (parent) {
			mTextPoint = parent->GetTextPointComp();
			mParentObject = parent;
			break;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay!!!!!!!!"));
}

// Called every frame
void AGameModeSwitchTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGameModeSwitchTrigger::OnOverlapBegin(
	UPrimitiveComponent *overlappedComp,
	AActor *otherActor,
	UPrimitiveComponent *otherComp,
	int32 otherBodyIndex,
	bool bFromSweep,
	const FHitResult &sweepResult)
{
	APlayerController *p = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (p) {
		AFirstPersonGameMode *gm = Cast<AFirstPersonGameMode>(
			UGameplayStatics::GetGameMode(GetWorld()));
 		if (gm && mTextPoint) {
			FVector loc = mTextPoint->GetActorLocation();
			FVector2D screenPos;
			bool result = p->ProjectWorldLocationToScreen(loc, screenPos);
			if (result) {
				UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));

				// �^�C�}�[����J�n 
				FOnTimeAttackStartDelegate delegate = FOnTimeAttackStartDelegate::CreateUObject(this, &AGameModeSwitchTrigger::OnStartTimeAttack);
				gm->SetTextLocation(screenPos);
				gm->StartTimer(5.0f, delegate);

				// �F�ς��J�n 
				AGameModeSwitchObject *parent = Cast<AGameModeSwitchObject>(mParentObject);
				if (parent) {
					parent->StartSwitching(gm->SwitchingPeriod());
				}
			}
		}
	}
}

void AGameModeSwitchTrigger::OnOverlapEnd(
	UPrimitiveComponent *overlappedComp,
	AActor *otherActor,
	UPrimitiveComponent *otherComp,
	int32 otherBodyIndex)
{
	APlayerController *p = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (p) {
		AFirstPersonGameMode *gm = Cast<AFirstPersonGameMode>(
			UGameplayStatics::GetGameMode(GetWorld()));
		if (gm) {
			UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd"));

			// �^�C�}�[���~ 
			gm->ResetTimer();

			// �F�ς����~  
			AGameModeSwitchObject *parent = Cast<AGameModeSwitchObject>(mParentObject);
			if (parent) {
				parent->StopSwitching();
			}
		}
	}
}

void AGameModeSwitchTrigger::OnStartTimeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStartTimeAttack"));
}

