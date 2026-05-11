// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WeaponTestActor.h"

AWeaponTestActor::AWeaponTestActor()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootScene);
}

void AWeaponTestActor::BeginPlay()
{
	Super::BeginPlay();

}


