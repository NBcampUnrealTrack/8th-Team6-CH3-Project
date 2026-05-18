// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectTypes.h"
#include "PlayerStatComponent.generated.h"

//-----------------------------------------------------------------------------
// DELEGATES
//-----------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHPChangedDelegate, float, CurrentHealth, float, MaxHealth,float, HealthDelta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM6_CH3_PROJECT_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerStatComponent();

protected:
	//-----------------------------------------------------------------------------
	// ENGINE OVERRIDES
	//-----------------------------------------------------------------------------
	virtual void BeginPlay() override;

public:
	//-----------------------------------------------------------------------------
	// STAT CONTROL METHODS
	//-----------------------------------------------------------------------------
	// 새로운 스탯 데이터로 초기화
	void SetStats(const FPlayerStatData& NewStats);

	//체력을 치유
	UFUNCTION(BlueprintCallable, Category = "Player|Stat")
	void HealHP(float Amount);

	// 데미지를 적용
	UFUNCTION(BlueprintCallable, Category = "Player|Stat")
	void ApplyDamage(float Amount);


	//  플레이어의 기본 스탯 데이터 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stat")
	FPlayerStatData PlayerStats;

	//-----------------------------------------------------------------------------
	// DELEGATE INSTANCES (EVENTS)
	//-----------------------------------------------------------------------------
	// 체력이 변경될 때마다 호출되는 델리게이트 (UI 업데이트용)
	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnHPChangedDelegate OnHPChanged;

	// 사망했을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnDeathDelegate OnDeath;
};