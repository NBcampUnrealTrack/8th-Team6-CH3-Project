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
	/*MaxAmmo = 30;
	CurrentAmmo = 30;
	AmmoPerFire = 1;*/
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
		//ATP_ThirdPersonCharacter* Player = Cast<ATP_ThirdPersonCharacter>(Character);
		//if (Player)
		//{
		//	ASandboxWeaponBase* SandboxWeapon = Cast<ASandboxWeaponBase>(this);
		//	if (SandboxWeapon)
		//	{
		//		Player->EquipWeapon(SandboxWeapon);
		//		SetActorEnableCollision(false);
		//	}
		//}


		ALunarAsylumCharacter* Player = Cast<ALunarAsylumCharacter>(Character);
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

void AWeaponBase::Use(ACharacter* Character)
{
	if (!Character) return;

	ATP_ThirdPersonCharacter* Player = Cast<ATP_ThirdPersonCharacter>(Character);
	if (!Player) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	AWeaponBase* Weapon = Character->GetWorld()->SpawnActor<AWeaponBase>(
		GetClass(),
		Character->GetActorLocation(),
		Character->GetActorRotation(),
		SpawnParams);

	if (Weapon)
	{
		Weapon->SetActorEnableCollision(false);
		ASandboxWeaponBase* SandboxWeapon = Cast<ASandboxWeaponBase>(Weapon);
		if (SandboxWeapon)
		{
			Player->EquipWeapon(SandboxWeapon);
		}
	}
}