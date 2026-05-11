// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectTypes.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FPlayerStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHP;

	FPlayerStatData() : MaxHP(100.f), CurrentHP(100.f) {}

	FPlayerStatData(float NewMaxHP) : MaxHP(NewMaxHP), CurrentHP(MaxHP) {}
};

USTRUCT(BlueprintType)
struct FAimSetting
{
	GENERATED_BODY()

	// 카메라 암 길이 (TargetArmLength)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultArmLength = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimArmLength = 90.f;

	// 소켓 오프셋 (SocketOffset - 어깨 시점 등 조절용) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultArmSocketOffset = { 0.f,50.f,50.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AimArmSocketOffset = { 0.f, 50.f,50.f };
	//  마우스 감도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSensitivity = 1.f; // 1.0배
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimSensitivity = 0.5f;  // 0.5배

	// 보간 속도 (줌이 얼마나 빠르게 될지)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultInterpSpeed = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimInterpSpeed = 8.f;

	// 시야각
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultFOV = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimFOV = 70.f;
};

USTRUCT(BlueprintType)
struct FMovementSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimSpeed = 150.f;
};

USTRUCT(BlueprintType)
struct FCharacterAnimMontages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Holster;

	FCharacterAnimMontages() : Reload(nullptr), Fire(nullptr), Equip(nullptr), Holster(nullptr){}
};

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	Unarmed     UMETA(DisplayName = "Unarmed"),   // 맨손(Normal)
	Primary     UMETA(DisplayName = "Primary"),   // 주무기
	Secondary   UMETA(DisplayName = "Secondary")  // 보조무기
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),        // 대기
	Equip       UMETA(DisplayName = "Equip"),       // 장착
	Holster     UMETA(DisplayName = "Holster"),     // 해제
	Reloading   UMETA(DisplayName = "Reloading"),   // 재장전
	Firing      UMETA(DisplayName = "Firing"),      // 발사
	HitReaction UMETA(DisplayName = "HitReaction"), // 피격
	Dead        UMETA(DisplayName = "Dead")			// 사망
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol		UMETA(DisplayName = "Pistol"),
	Rifle		UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun"),
};
