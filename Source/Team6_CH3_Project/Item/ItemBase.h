// ItemBase.h
#pragma once

#include "Engine/Texture2D.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Consumable	UMETA(DisplayName = "Consumable"),
	Ammo		UMETA(DisplayName = "Ammo")
};

UCLASS()
class TEAM6_CH3_PROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* HoverIcon;

	UFUNCTION(BlueprintCallable)
	virtual void Interact(ACharacter* Character);

	UFUNCTION(BlueprintCallable)
	virtual void Use(ACharacter* Character);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Stack")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Stack")
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Stack")
	int32 Quantity = 1;

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
