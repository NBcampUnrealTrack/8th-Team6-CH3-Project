// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LunarAsylumCharacter.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API ALunarAsylumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALunarAsylumCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	// Jump Input Action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpAction;

	// Move Input Action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	// 조준상태
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsAiming;

	// 기본 감도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSensitivity;

	// 조준 감도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimSensitivity;


	/** Called for movement input */
	void Move(const struct FInputActionValue& Value);

	/** Called for looking input */
	void Look(const struct FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;
};
