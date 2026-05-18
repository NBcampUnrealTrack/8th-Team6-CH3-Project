// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LunarAsylumCharacterController.h"
#include "Player/InteractionWidget.h"
#include "../UI/CombatHUDWidget.h"


void ALunarAsylumCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UCombatHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(this, InteractionWidgetClass);
		if (InteractionWidget)
		{
			InteractionWidget->AddToViewport();
		}
	}
}
