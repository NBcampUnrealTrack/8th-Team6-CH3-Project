// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ProjectTypes.h"
#include "ANS_SetEquipState.generated.h"

/**
 * 
 */
UCLASS()
class TEAM6_CH3_PROJECT_API UANS_SetEquipState : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	EEquipState StateToSet;
};
