// SandboxWeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ProjectTypes.h"
#include "SandboxWeaponBase.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API ASandboxWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

	void StartFire();

	void StopFire();

	UFUNCTION(BlueprintImplementableEvent)
	void SandboxFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* EmptySound;

	UFUNCTION(BlueprintCallable)
	bool CanAttack();

	void MaxAmmoAdd();
protected:

	UFUNCTION(BlueprintCallable)
	void LinetraceOneShot(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void LinetraceSpread(FVector Direction, int32 PellectCount, float SpreadAngle);

	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();

	UFUNCTION(BlueprintCallable)
	void ApplyRecoil();

	FTimerHandle TimerReloadDelay;

	// 라인트레이스 시작 지점 보정 오프셋 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ViewStartOffset = 175.f;

	//-----------------------------------------------------------------------------
	// 반동 Pitch,Yaw(Min,Max)
	//-----------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float PitchRecoilMin = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float PitchRecoilMax = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float YawRecoilMin = -0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float YawRecoilMax = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool showDebug = false;

	bool bIsTryingToFire;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerFireDelay;

	UFUNCTION()
	void HandleFireDelay();
};
