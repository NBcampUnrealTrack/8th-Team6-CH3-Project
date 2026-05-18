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

	FORCEINLINE class UCombatHUDWidget* GetHUDWidget() const { return HUDWidget; }
	FORCEINLINE class UInteractionWidget* GetInteractionWidget() const { return InteractionWidget; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Class")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Class")
	TSubclassOf<UUserWidget> InteractionWidgetClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widget|Instance")
	class UCombatHUDWidget* HUDWidget = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widget|Instance")
	class UInteractionWidget* InteractionWidget = nullptr;
};
