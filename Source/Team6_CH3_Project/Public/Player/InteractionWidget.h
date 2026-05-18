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
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GuideText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AcquisitionText;

	virtual void NativeConstruct() override;

public:

	void ShowGuide(const FString& ItemName);

	void HideGuide();

	UFUNCTION()
	void ShowAcquisitionMessage(const FString& ItemName);
	UFUNCTION()
	void HandleTargetItemChanged(const FString& ItemName);

	void HideAcquisitionMessage();


	FTimerHandle AcquisitionTimerHandle;

	UPROPERTY(BlueprintReadOnly)
	class ALunarAsylumCharacter* LunarAsylumCharacter = nullptr;
};
