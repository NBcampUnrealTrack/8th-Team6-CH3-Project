// WeaponBase.h
#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "WeaponBase.generated.h"

//UENUM(BlueprintType)
//enum class EWeaponType : uint8
//{
//	Rifle	 UMETA(DisplayName = "Rifle"),
//	Shotgun	 UMETA(DisplayName = "Shotgun"),
//	Pistol	 UMETA(DisplayName = "Pistol")
//};

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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//EWeaponType WeaponType = EWeaponType::Rifle;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();


	UFUNCTION(BlueprintCallable)
	float GetRoF() const
	{
		return RoF;
	}

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
