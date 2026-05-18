// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SynopsisDockWidget.h"

void USynopsisDockWidget::StartStoryTyping()
{
	CurrentCharIndex = 0;
	CurrentDisplayedText = "";

	GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &USynopsisDockWidget::TypeNextChar,0.05f,true);
}

void USynopsisDockWidget::TypeNextChar()
{
	if (CurrentCharIndex < FullStoryText.Len())
	{
		CurrentDisplayedText.AppendChar(FullStoryText[CurrentCharIndex]);
		CurrentCharIndex++;

		OnTextUpdate(CurrentDisplayedText);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
		OnIntroFinish();
	}
}