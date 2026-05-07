// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/ArrowComponent.h"
#include "../ItemBase.h"
#include "../../TP_ThirdPerson/TP_ThirdPersonCharacter.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(Root);

	CanFire = true;
	RoF = 0.4f;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Fire()
{

}

void AWeaponBase::HandleFireDelay()
{
	CanFire = true;
}

void AWeaponBase::Interact(ACharacter* Character)
{
	if (Character)
	{
		ATP_ThirdPersonCharacter* Player = Cast<ATP_ThirdPersonCharacter>(Character);
		if (Player)
		{
			ASandboxWeaponBase* SandboxWeapon = Cast<ASandboxWeaponBase>(this);
			if (SandboxWeapon)
			{
				Player->EquipWeapon(SandboxWeapon);
				SetActorEnableCollision(false);
			}
		}
	}
}