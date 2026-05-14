// WeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "ProjectTypes.h"
#include "Components\SphereComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> FirePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	FCharacterAnimMontages CharacterAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform EquipOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform HolsterOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* MuzzleEffect;


	UFUNCTION(BlueprintCallable)
	virtual void Fire();


	UFUNCTION(BlueprintCallable)
	float GetRoF() const { return RoF; }

	bool GetCanFire() const { return CanFire; }


	virtual void Interact(ACharacter* Character) override;
	virtual void Use(ACharacter* Character) override;
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
