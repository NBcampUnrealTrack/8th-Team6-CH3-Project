// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatComponent.h"


UPlayerStatComponent::UPlayerStatComponent() : PlayerStats(100.f) {}

void UPlayerStatComponent::SetStats(const FPlayerStatData& NewStats)
{
	PlayerStats = NewStats;
}

void UPlayerStatComponent::HealHP(float Amount)
{
	PlayerStats.CurrentHP = FMath::Clamp(PlayerStats.CurrentHP + Amount, 0.f, PlayerStats.MaxHP);

	OnHPChanged.Broadcast(PlayerStats.CurrentHP, PlayerStats.MaxHP, Amount);
}

void UPlayerStatComponent::ApplyDamage(float Amount)
{
	PlayerStats.CurrentHP = FMath::Clamp(PlayerStats.CurrentHP - Amount, 0.f, PlayerStats.MaxHP);
	UE_LOG(LogTemp, Log, TEXT("UPlayerStatComponent::ApplyDamage"));
	OnHPChanged.Broadcast(PlayerStats.CurrentHP, PlayerStats.MaxHP, -Amount);

	if (PlayerStats.CurrentHP <= 0.f)
	{
		OnDeath.Broadcast();
	}
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

