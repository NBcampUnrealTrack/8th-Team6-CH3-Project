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

protected:
	UFUNCTION(BlueprintCallable)
	bool CheckAmmo();

	UFUNCTION(BlueprintCallable)
	void LinetraceOneShot(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();
};
