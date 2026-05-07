// SandboxWeaponBase
#include "SandboxWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

void ASandboxWeaponBase::Fire()
{
	if (!CanFire) return;
	SandboxFire();

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
	CurrentAmmo = MaxAmmo;
}

bool ASandboxWeaponBase::CheckAmmo()
{
	return CurrentAmmo >= AmmoPerFire;
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
		0.3f,
		0,
		2.0f
	);
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

void ASandboxWeaponBase::UpdateAmmo()
{
}
