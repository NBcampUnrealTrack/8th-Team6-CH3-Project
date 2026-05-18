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

	UFUNCTION(BlueprintImplementableEvent)
	void SandboxFire();

	//UFUNCTION(BlueprintCallable)
	//void Reload();

	// 사운드 사용안함, 애니메이션 몽타주에서 사용
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USoundBase* ReloadStartSound;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USoundBase* ReloadEndSound;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USoundBase* EmptySound;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float RecoilAmount = 1.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float ReloadTime = 2.0f;


	////////  추가 ///////////////
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	//FCharacterAnimMontages CharacterAnimMontages;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
	//EWeaponType WeaponType;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UCameraShakeBase> CameraShakeClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector EquipOffset;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector HolsterOffset;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	//class UNiagaraSystem* MuzzleEffect;

	UFUNCTION(BlueprintCallable)
	bool CanAttack();

	void MaxAmmoAdd();
protected:

	UFUNCTION(BlueprintCallable)
	void LinetraceOneShot(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void LinetraceSpread(FVector Direction, int32 PellectCount, float SpreadAngle);

	//UFUNCTION(BlueprintCallable)
	//void PlaySound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();

	UFUNCTION(BlueprintCallable)
	void ApplyRecoil();

	//UFUNCTION()
	//void FinishReload();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReloading = false;

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
};
