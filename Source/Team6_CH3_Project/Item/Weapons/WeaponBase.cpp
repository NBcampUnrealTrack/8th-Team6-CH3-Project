// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/ArrowComponent.h"
#include "TP_ThirdPerson/LunarAsylumCharacter.h"
#include "../ItemBase.h"
#include "../../TP_ThirdPerson/TP_ThirdPersonCharacter.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(Mesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Mesh);

	CanFire = true;
	RoF = 0.4f;
}

void AWeaponBase::Fire()
{

}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}
