// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectTypes.h"
#include "PlayerStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHPChangedDelegate, float, CurrentHealth, float, MaxHealth,float, HealthDelta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM6_CH3_PROJECT_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

	void SetStats(const FPlayerStatData& NewStats);
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void HealHP(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void ApplyDamage(float Amount);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FPlayerStatData PlayerStats;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHPChangedDelegate OnHPChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathDelegate OnDeath;
};
