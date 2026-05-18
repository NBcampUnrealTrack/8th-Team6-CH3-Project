// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player\WeaponTestActor.h"
#include "ProjectTypes.h"
#include "../Item/ItemBase.h"
#include "../Item/Weapons/SandboxWeaponBase.h"
#include "LunarAsylumCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetItemChangedSignature, const FString&, ItemName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAcquiredSignature, const FString&, ItemAcquiredName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipStateChanged, EEquipState, EquipState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimingChanged, bool, bIsAiming);

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

	// 조준
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAiming;

	// 조준 관련 설정 값들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Aim")
	FAimSetting AimSettings;
	float CurrentAimSensitivity;

	// 달리기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprint;

	// 이동 관련 설정 값들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
	FMovementSetting MoveSettings;

	// 캐릭터 상태 - 맨손/주무기/보조무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EEquipState CurrentEquipState;

	// 애니메이션 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EActionState CurrentActionState;

	// 주무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASandboxWeaponBase* PrimaryWeapon;
	// 보조무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASandboxWeaponBase* SecondaryWeapon;
	// 현재 착용중인 무기를 가리키는 포인터
	UPROPERTY(BlueprintReadOnly)
	ASandboxWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintAssignable)
	FEquipStateChanged OnEquipStateChanged;
	UPROPERTY(BlueprintAssignable)
	FActionStateChanged OnActionStateChanged;

	UPROPERTY(BlueprintAssignable)
	FAimingChanged OnAimingChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AM_HitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AM_Death;

	class UInventoryComponent* InventoryComponent;

	// 인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 SelectedSlotIndex = -1;

	FTimerHandle AutoFireTimer;

	FOnTargetItemChangedSignature OnTargetItemChanged;

	FOnItemAcquiredSignature OnItemAcquired;

	// 이동
	void Move(const struct FInputActionValue& Value);

	// 마우스 입력 기반 시점 회전
	void Look(const struct FInputActionValue& Value);

	void SetActionState(EActionState NewState);

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

	// 아이템 사용
	void UseItem();

	// 재장전
	void Reload();

	// 주무기 장착/해제
	void PrimaryEquipToggle();

	// 보조무기 장착/해제
	void SecondaryEquipToggle();

	// 피격
	void HitReaction();

	// 사망
	UFUNCTION()
	void OnDeath();

	public:
	void EquipWeapon(ASandboxWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable)
	void ANAttachWeapon();

	UFUNCTION(BlueprintCallable)
	void ANHolsterWeapon();

	void InternalAttachWeapon(ASandboxWeaponBase* Weapon, FName SocketName, const FTransform& Offset);

	// 무기위치,회전 보간 관련 변수
	bool bIsInterpWeaponTransform = false;
	FTransform TargetWeaponTransform;

	UPROPERTY(EditAnywhere)
	float WeaponInterpSpeed = 10.0f; // 보간 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IK")
	float LeftHandIKAlpha = 0.f;
	float TargetIKAlpha = 0.f;
	UPROPERTY(EditAnywhere)
	float IKInterpSpeed = 10.0f; // 보간 속도
	bool bIsIKAlpha = false;

	void UpdateWeaponTransform(float DeltaTime);

	void UpdateWeaponIKTransform();

	void UpdateWeaponIKWeight(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK")
	FTransform WeaponEffector;

	void OnEquipMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	void UpdatePlayerStateDebugMessage();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	AItemBase* TargetItem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionDistance = 1000.f;

	void UpdateInteractionCheck();

};
