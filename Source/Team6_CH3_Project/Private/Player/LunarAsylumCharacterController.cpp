// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LunarAsylumCharacterController.h"
#include "../UI/CombatHUDWidget.h"


void ALunarAsylumCharacterController::BeginPlay()
{
	Super::BeginPlay();
	if (HUDWidgetClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Check1"));
		HUDWidget = CreateWidget<UCombatHUDWidget>(this, HUDWidgetClass);
		//HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			UE_LOG(LogTemp, Log, TEXT("Check2"));
			HUDWidget->AddToViewport();
		}
	}
}
