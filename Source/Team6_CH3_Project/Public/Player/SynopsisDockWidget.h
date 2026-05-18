// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SynopsisDockWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM6_CH3_PROJECT_API USynopsisDockWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro")
	FString FullStoryText;

	FString CurrentDisplayedText;
	int32 CurrentCharIndex = 0;
	FTimerHandle TypingTimerHandle;

	UFUNCTION(BlueprintImplementableEvent, Category = "Intro")
	void OnTextUpdate(const FString& NewText);

	UFUNCTION(BlueprintImplementableEvent, Category = "Intro")
	void OnIntroFinish();

	void TypeNextChar();

	UFUNCTION(BlueprintCallable, Category = "Intro")
	void StartStoryTyping();
};
