// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LunarAsylumCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TEAM6_CH3_PROJECT_API ALunarAsylumCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;
	class UCombatHUDWidget* HUDWidget = nullptr;
	//class UUserWidget* HUDWidget = nullptr;
protected:
	virtual void BeginPlay() override;

};
