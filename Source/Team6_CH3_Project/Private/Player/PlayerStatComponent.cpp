// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatComponent.h"


UPlayerStatComponent::UPlayerStatComponent() : PlayerStats(100.f) {}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::SetStats(const FPlayerStatData& NewStats)
{
	PlayerStats = NewStats;

	// 스탯이 통째로 바뀐 경우에도 UI가 한 번 갱신될 수 있도록 초기 변화량 전달
	OnHPChanged.Broadcast(PlayerStats.CurrentHP, PlayerStats.MaxHP, 0.f);
}

void UPlayerStatComponent::HealHP(float Amount)
{
	if (Amount <= 0.f || PlayerStats.CurrentHP >= PlayerStats.MaxHP)
	{
		return;
	}

	float PreviousHP = PlayerStats.CurrentHP;
	PlayerStats.CurrentHP = FMath::Clamp(PlayerStats.CurrentHP + Amount, 0.f, PlayerStats.MaxHP);

	float HealthDelta = PlayerStats.CurrentHP - PreviousHP;

	OnHPChanged.Broadcast(PlayerStats.CurrentHP, PlayerStats.MaxHP, HealthDelta);
}

void UPlayerStatComponent::ApplyDamage(float Amount)
{
	if (Amount <= 0.f || PlayerStats.CurrentHP <= 0.f)
	{
		return;
	}

	float PreviousHP = PlayerStats.CurrentHP;
	PlayerStats.CurrentHP = FMath::Clamp(PlayerStats.CurrentHP - Amount, 0.f, PlayerStats.MaxHP);

	float HealthDelta = PlayerStats.CurrentHP - PreviousHP;

	OnHPChanged.Broadcast(PlayerStats.CurrentHP, PlayerStats.MaxHP, HealthDelta);

	if (PlayerStats.CurrentHP <= 0.f)
	{
		OnDeath.Broadcast();
	}
}

