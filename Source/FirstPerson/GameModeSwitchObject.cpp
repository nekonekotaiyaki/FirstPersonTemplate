// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSwitchObject.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AGameModeSwitchObject::AGameModeSwitchObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(_TEXT("MeshComponent"));
	RootComponent = mMeshComponent;


	mIsLocked = false;
}

// Called when the game starts or when spawned
void AGameModeSwitchObject::BeginPlay()
{
	Super::BeginPlay();


	if (mDefaultMaterial) {
		mMeshComponent->SetMaterial(0, mDefaultMaterial);
	}
}

// Called every frame
void AGameModeSwitchObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

