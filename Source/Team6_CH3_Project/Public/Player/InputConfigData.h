// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class TEAM6_CH3_PROJECT_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 점프
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpAction;

	// 이동
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	// 마우스 회전
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	// 달리기
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* SprintAction;

	// 조준
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* AimAction;

	// 공격
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireAction;

	// 주무기
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* PrimaryAction;

	// 보조무기
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* SecondaryAction;

	// 재장전
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ReloadAction;

	// 인벤토리
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InventoryAction;

	// 상호작용
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InteractAction;
};
