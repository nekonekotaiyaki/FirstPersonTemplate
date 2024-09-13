// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonPlayerController.generated.h"

class UInputMappingContext;


/**
 *
 */
UCLASS()
class FIRSTPERSON_API AFirstPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFirstPersonPlayerController();

	bool IsTimeAttackMode();
	void SetBoostShotPower(float power);
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface


private:
	bool	mIsTimeAttackMode;
};
