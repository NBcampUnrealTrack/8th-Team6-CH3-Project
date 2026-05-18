// SandboxWeaponBase
#include "SandboxWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

void ASandboxWeaponBase::Fire()
{
	//if (CanAttack()) return;

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

//void ASandboxWeaponBase::Reload()
//{
//	if (bIsReloading) return;
//	if (CurrentAmmo == MaxAmmo) return;
//	
//	PlaySound(ReloadStartSound);
//
//	bIsReloading = true;
//	CanFire = false;
//
//	float SoundDuration = ReloadStartSound ? ReloadStartSound->GetDuration() : ReloadTime;
//
//	GetWorldTimerManager().SetTimer(
//		TimerReloadDelay,
//		this,
//		&ASandboxWeaponBase::FinishReload,
//		SoundDuration,
//		false
//	);
//}

//void ASandboxWeaponBase::FinishReload()
//{
//	CurrentAmmo = MaxAmmo;
//	bIsReloading = false;
//	CanFire = true;
//
//	PlaySound(ReloadEndSound);
//}

//bool ASandboxWeaponBase::CheckAmmo()
//{
//	if (CurrentAmmo < AmmoPerFire)
//	{
//		//PlaySound(EmptySound);
//		return false;
//	}
//	return true;
//}

bool ASandboxWeaponBase::CanAttack()
{
	if (CurrentAmmo < AmmoPerFire || !CanFire)
	{
		return false;
	}

	return true;
}

void ASandboxWeaponBase::LinetraceOneShot(FVector Direction)
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;

	// 카메라의 현재 위치와 회전값 가져오기
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	// ViewStartOffset 만큼 시작 지점을 앞으로 전진
	ViewPointLocation = ViewPointLocation + ViewPointRotation.Vector() * ViewStartOffset;

	// 시작 지점으로부터 발사 거리(Range)만큼 최종 도달 지점 계산
	FVector End = ViewPointLocation + ViewPointRotation.Vector() * Range;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ViewPointLocation,
		End,
		ECC_Visibility,
		Params
	);

	// 판정용 트레이스 : 디버그 출력
	DrawDebugLine(
		GetWorld(),
		ViewPointLocation,
		End,
		FColor::Red,
		false,
		5.f,
		0,
		2.0f
	);

	FVector MuzzleLocation = FirePoint->GetComponentLocation();
	FVector VisualTraceEnd;

	if (bHit) 
	{
		VisualTraceEnd = HitResult.ImpactPoint; 
	}
	else 
	{
		VisualTraceEnd = HitResult.TraceEnd; 
	}

	// 연출용 트레이스 : 디버그 출력
	DrawDebugLine(
		GetWorld(),
		MuzzleLocation,
		VisualTraceEnd,
		FColor::Blue,
		false,
		5.f,
		0,
		2.0f
	);

	// 라인트레이스에 히트시 태그로 대미지 처리
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

void ASandboxWeaponBase::LinetraceSpread(FVector Direction, int32 PellectCount, float SpreadAngle)
{
	for (int32 i = 0; i < PellectCount; i++)
	{
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		// 카메라의 현재 위치와 회전값 가져오기
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

		// ViewStartOffset 만큼 시작 지점을 앞으로 전진
		ViewPointLocation = ViewPointLocation + ViewPointRotation.Vector() * ViewStartOffset;

		// 탄 퍼짐(Bullet Spread) 방향(원뿔 모양의 가상 공간(Cone) 안에서 무작위 방향 벡터(VRandCone)를 추출)
		FVector SpreadDir = FMath::VRandCone(ViewPointRotation.Vector(), FMath::DegreesToRadians(SpreadAngle));

		// 시작 지점으로부터 발사 거리(Range)만큼 최종 도달 지점 계산
		FVector End = ViewPointLocation + SpreadDir * Range;

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			ViewPointLocation,
			End,
			ECC_Visibility,
			Params
		);


		// 판정용 트레이스 : 디버그 출력
		DrawDebugLine(
			GetWorld(),
			ViewPointLocation,
			End,
			FColor::Red,
			false,
			5.f,
			0,
			2.0f
		);

		FVector MuzzleLocation = FirePoint->GetComponentLocation();
		FVector VisualTraceEnd;

		if (bHit)
		{
			VisualTraceEnd = HitResult.ImpactPoint;
		}
		else
		{
			VisualTraceEnd = HitResult.TraceEnd;
		}

		// 연출용 트레이스 : 디버그 출력
		DrawDebugLine(
			GetWorld(),
			MuzzleLocation,
			VisualTraceEnd,
			FColor::Blue,
			false,
			5.f,
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

//void ASandboxWeaponBase::PlaySound(USoundBase* Sound)
//{
//	if (Sound)
//	{
//		UGameplayStatics::PlaySoundAtLocation(
//			this,
//			Sound,
//			GetActorLocation()
//		);
//	}
//}

void ASandboxWeaponBase::ApplyRecoil()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		float YawRecoil = FMath::FRandRange(YawRecoilMin, YawRecoilMax);
		float PitchRecoil = FMath::FRandRange(PitchRecoilMin, PitchRecoilMax);

		PC->AddPitchInput(-PitchRecoil);
		PC->AddYawInput(YawRecoil);
	}
}



void ASandboxWeaponBase::UpdateAmmo()
{
	CurrentAmmo -= AmmoPerFire;
}


