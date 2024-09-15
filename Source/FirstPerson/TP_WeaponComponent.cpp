// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FirstPersonCharacter.h"
#include "FirstPersonProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "FirstPersonPlayerController.h"
#include "TimeAttackGameMode.h"
#include "TimerManager.h"

#define	BOOST_TIMER_SPEED		(1.0f / 60.0f)

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			AFirstPersonProjectile *projectile = World->SpawnActor<AFirstPersonProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (projectile) {
				AFirstPersonPlayerController *pc = Cast<AFirstPersonPlayerController>(PlayerController);
				bool isTimeAttackMode = (pc) ? pc->IsTimeAttackMode() : false;
				projectile->SetTimeAttackMode(isTimeAttackMode);

				// コールバック設定 
				AGameModeBase *gm = World->GetAuthGameMode();
				if (gm) {
					ATimeAttackGameMode *timeAttack = Cast<ATimeAttackGameMode>(gm);
					if (timeAttack) {
						timeAttack->SetOnGetScoreCallback(projectile);
					}
				}
			}
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(AFirstPersonCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			// BoostFire
			EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::OnBoostButtonPressed);
			EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::OnBoostButtonReleased);
			EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Canceled, this, &UTP_WeaponComponent::OnBoostButtonReleased);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::BoostFire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld *const World = GetWorld();
		if (World != nullptr)
		{
			// 溜めた値で弾数を決める 
			mNumBoostShots = FMath::CeilToInt(mBoostPower * 10.0f);
			
			// スポーン時に衝突するのでタイミングをずらす 
			const float GAP = 0.1f;
			World->GetTimerManager().SetTimer(
				mBoostShotTimerHandle,
				this,
				&UTP_WeaponComponent::OnUpdateBoostShot,
				GAP,
				true);
		}
	}
}
void UTP_WeaponComponent::OnBoostButtonPressed()
{
	mIsBoostRunning = true;
	mBoostStartTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(
		mBoostTimerHandle,
		this,
		&UTP_WeaponComponent::OnUpdateBoost,
		BOOST_TIMER_SPEED,
		true);
	//UE_LOG(LogTemp, Warning, TEXT("OnBoostButtonPressed"));
}
void UTP_WeaponComponent::OnBoostButtonReleased()
{
	mIsBoostRunning = false;
	GetWorld()->GetTimerManager().ClearTimer(mBoostTimerHandle);
	BoostFire();
	mBoostPower = 0.0f;
	if (Character && Character->GetController()) {
		AFirstPersonPlayerController *p = Cast<AFirstPersonPlayerController>(Character->GetController());
		if (p) {
			p->SetBoostShotPower(0.0f);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("OnBoostButtonReleased"));
}
void UTP_WeaponComponent::OnUpdateBoost()
{
	float elapsedTime = GetWorld()->GetTimeSeconds() - mBoostStartTime;
	float power = elapsedTime / 1.0f;
	mBoostPower = FMath::Clamp(power, 0.0f, 1.0f);

	//UE_LOG(LogTemp, Warning, TEXT("Boost Progress: %f"), power);
	if (Character && Character->GetController()) {
		AFirstPersonPlayerController *p = Cast<AFirstPersonPlayerController>(Character->GetController());
		if (p) {
			p->SetBoostShotPower(mBoostPower);
		}
	}
}
void UTP_WeaponComponent::OnUpdateBoostShot()
{
	UWorld *const World = GetWorld();
	if (mNumBoostShots > 0) {
		AFirstPersonPlayerController *PlayerController = Cast<AFirstPersonPlayerController>(Character->GetController());

		// 弾が飛ぶ向きを取得
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

		// バラけさせる
		FRotator spreadDir = SpawnRotation;
		spreadDir.Yaw += FMath::RandRange(-15.0f, 15.0f);	// 左右
		spreadDir.Pitch += FMath::RandRange(-15.0f, 15.0f);	// 上下
		if (spreadDir.Pitch < -10.0f) {
			spreadDir.Pitch += 10.0f;
		}

		// 発射位置
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		AFirstPersonProjectile *projectile = World->SpawnActor<AFirstPersonProjectile>(ProjectileClass, SpawnLocation, spreadDir, ActorSpawnParams);
		if (projectile) {
			projectile->SetTimeAttackMode(PlayerController->IsTimeAttackMode());
			projectile->SetSuperProjectileMode(true);

			// コールバック設定
			AGameModeBase *gm = World->GetAuthGameMode();
			if (gm) {
				ATimeAttackGameMode *timeAttack = Cast<ATimeAttackGameMode>(gm);
				if (timeAttack) {
					timeAttack->SetOnGetScoreCallback(projectile);
				}
			}
		}

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance *AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		// 減らす 
		mNumBoostShots--;
	}
	else {
		World->GetTimerManager().ClearTimer(mBoostShotTimerHandle);
	}
}
