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

	UFUNCTION(BlueprintCallable)
	void Reload();

	// 사운드 사용안함, 애니메이션 몽타주에서 사용
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
