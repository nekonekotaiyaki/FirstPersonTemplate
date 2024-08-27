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


    // ���b�V�� 
    UPROPERTY(VisibleAnywhere, Category = "StaticMesh")
    UStaticMeshComponent* mMeshComponent;

    // �}�e���A�� 
    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterial* mDefaultMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterial* mLockedMaterial;

private:


    // �t���O 
    bool	mIsLocked;
};
