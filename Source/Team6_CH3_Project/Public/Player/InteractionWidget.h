// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\TextBlock.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM6_CH3_PROJECT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// 상호작용 가능할 때 뜨는 가이드
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GuideText;

	// 아이템을 획득했을 때 뜨는 알림 문구
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AcquisitionText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ItemDisplayDuration = 2.5f;

public:

	// 조준하거나 바라보는 타겟 아이템이 바뀌었을 때 델리게이트를 통해 호출
	UFUNCTION()
	void HandleTargetItemChanged(const FString& ItemName);

	void ShowGuide(const FString& ItemName);

	void HideGuide();

	UFUNCTION()
	void ShowAcquisitionMessage(const FString& ItemName);

	void HideAcquisitionMessage();

private:

	// 획득 문구를 몇 초 뒤에 자동으로 사라지게 만들 타이머 핸들 
	FTimerHandle AcquisitionTimerHandle;
};