// SandboxWeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SandboxWeaponBase.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API ASandboxWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SandboxFire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ReloadStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ReloadEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* EmptySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilAmount = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime = 2.0f;

protected:
	UFUNCTION(BlueprintCallable)
	bool CheckAmmo();

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

	UFUNCTION()
	void FinishReload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReloading = false;

	FTimerHandle TimerReloadDelay;
};
