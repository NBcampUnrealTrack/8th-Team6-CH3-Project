// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SynopsisDockWidget.h"

void USynopsisDockWidget::StartStoryTyping()
{
	if (FullStoryText.IsEmpty())
	{
		OnIntroFinish();
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);

	CurrentCharIndex = 0;
	CurrentDisplayedText = "";

	GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &USynopsisDockWidget::TypeNextChar, TypingSpeed, true);
}

void USynopsisDockWidget::TypeNextChar()
{
	if (CurrentCharIndex >= FullStoryText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
		OnIntroFinish();
		return;
	}

	CurrentDisplayedText += FullStoryText.Mid(CurrentCharIndex, 1);
	CurrentCharIndex++;

	OnTextUpdate(CurrentDisplayedText);
}