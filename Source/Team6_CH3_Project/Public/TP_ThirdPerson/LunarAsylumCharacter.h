// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player\WeaponTestActor.h"
#include "ProjectTypes.h"
#include "LunarAsylumCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipStateChanged, EEquipState, EquipState);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionStateChanged, EActionState, ActionState);

UCLASS()
class TEAM6_CH3_PROJECT_API ALunarAsylumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALunarAsylumCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class UPlayerStatComponent* PlayerStatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputConfigData* InputConfigData;

	// 조준상태
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAiming;

	// 조준 관련 설정 값들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Aim")
	FAimSetting AimSettings;
	float CurrentAimSensitivity;

	// 이동 관련 설정 값들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
	FMovementSetting MoveSettings;

	// 맨손/주무기/보조무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EEquipState CurrentEquipState;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EActionState CurrentActionState;

	EActionState PreviousActionState;

	// 주무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeaponTestActor* PrimaryWeapon;
	// 보조무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeaponTestActor* SecondaryWeapon;
	// 현재 착용중인 무기를 가리키는 포인터
	UPROPERTY(BlueprintReadOnly)
	AWeaponTestActor* CurrentWeapon;

	UPROPERTY(BlueprintAssignable)
	FEquipStateChanged OnEquipStateChanged;
	UPROPERTY(BlueprintAssignable)
	FActionStateChanged OnActionStateChanged;

	// 이동
	void Move(const struct FInputActionValue& Value);

	// 마우스 입력 기반 시점 회전
	void Look(const struct FInputActionValue& Value);

	void SetActionState(EActionState NewState);
	void RestorePreviousState();

	void SetEquipState(EEquipState NewState);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Amount);

	// 조준
	UFUNCTION(BlueprintCallable)
	void StartAim();
	UFUNCTION(BlueprintCallable)
	void StopAim();
	void AimSetting();
	void UpdateAimZoom(float DeltaTime);

	// 공격
	UFUNCTION(BlueprintCallable)
	void OnFire();
	UFUNCTION(BlueprintCallable)
	void StartFire();
	UFUNCTION(BlueprintCallable)
	void StopFire();

	// 달리기
	void StartSprint();
	void StopSprint();

	// 상호작용 
	void Interact();

	// 인벤토리
	void InventoryToggle();

	// 재장전
	void Reload();

	// 주무기 장착/해제
	void PrimaryEquipToggle();

	// 보조무기 장착/해제
	void SecondaryEquipToggle();
};
