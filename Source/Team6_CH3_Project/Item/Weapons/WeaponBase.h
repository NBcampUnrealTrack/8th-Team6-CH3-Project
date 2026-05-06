// WeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "WeaponBase.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
	
public:
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> FirePoint;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	virtual void Interact(ACharacter* Character) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoPerFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerHit;

	UPROPERTY(BlueprintReadWrite)
	bool CanFire;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerFireDelay;

	UFUNCTION()
	void HandleFireDelay();
};
