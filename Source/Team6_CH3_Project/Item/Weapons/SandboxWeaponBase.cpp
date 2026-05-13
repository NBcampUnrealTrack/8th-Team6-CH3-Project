// SandboxWeaponBase
#include "SandboxWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

void ASandboxWeaponBase::Fire()
{
	if (!CanFire) return;
	SandboxFire();
	ApplyRecoil();

	CanFire = false;
	GetWorldTimerManager().SetTimer(
		TimerFireDelay,
		this,
		&ASandboxWeaponBase::HandleFireDelay,
		RoF,
		false
	);
}

void ASandboxWeaponBase::Reload()
{
	if (bIsReloading) return;
	if (CurrentAmmo == MaxAmmo) return;
	
	PlaySound(ReloadStartSound);

	bIsReloading = true;
	CanFire = false;

	float SoundDuration = ReloadStartSound ? ReloadStartSound->GetDuration() : ReloadTime;

	GetWorldTimerManager().SetTimer(
		TimerReloadDelay,
		this,
		&ASandboxWeaponBase::FinishReload,
		SoundDuration,
		false
	);
}

void ASandboxWeaponBase::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
	CanFire = true;

	PlaySound(ReloadEndSound);
}

bool ASandboxWeaponBase::CheckAmmo()
{
	if (CurrentAmmo < AmmoPerFire)
	{
		PlaySound(EmptySound);
		return false;
	}
	return true;
}

void ASandboxWeaponBase::LinetraceOneShot(FVector Direction)
{
	FHitResult HitResult;
	FVector Start = FirePoint->GetComponentLocation();
	FVector End = Start + Direction * Range;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Red,
		false,
		0.2f,
		0,
		2.0f
	);
}

void ASandboxWeaponBase::LinetraceSpread(FVector Direction, int32 PellectCount, float SpreadAngle)
{
	for (int32 i = 0; i < PellectCount; i++)
	{
		FVector SpreadDir = FMath::VRandCone(Direction, FMath::DegreesToRadians(SpreadAngle));

		FHitResult HitResult;
		FVector Start = FirePoint->GetComponentLocation();
		FVector End = Start + SpreadDir * Range;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		//GetWorld()->
		//Params.AddIgnoredActor()

		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			Params
		);

		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Red,
			false,
			10.2f,
			0,
			2.0f
		);

		if (HitResult.GetActor())
		{
			AActor* HitActor = HitResult.GetActor();

			if (HitActor->ActorHasTag(FName(TEXT("Enemy"))))
			{
				UGameplayStatics::ApplyDamage(HitActor, DamagePerHit, nullptr, this, UDamageType::StaticClass());
				UE_LOG(LogTemp, Log, TEXT("%s : %f 피해"), *HitResult.GetActor()->GetActorLabel(), DamagePerHit);
			}

		}
	}
}

void ASandboxWeaponBase::PlaySound(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound,
			GetActorLocation()
		);
	}
}

void ASandboxWeaponBase::ApplyRecoil()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->AddPitchInput(-RecoilAmount);
	}
}



void ASandboxWeaponBase::UpdateAmmo()
{
	CurrentAmmo -= AmmoPerFire;
}


