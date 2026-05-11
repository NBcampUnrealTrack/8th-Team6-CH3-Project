// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectTypes.h"
#include "WeaponTestActor.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API AWeaponTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponTestActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	FCharacterAnimMontages CharacterAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector EquipOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HolsterOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* MuzzleEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
