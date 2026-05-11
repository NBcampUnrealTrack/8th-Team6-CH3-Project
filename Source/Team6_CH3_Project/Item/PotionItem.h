#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PotionItem.generated.h"

UCLASS()
class TEAM6_CH3_PROJECT_API APotionItem : public AItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotionItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	float HealAmount = 30.f;

	virtual void Use(ACharacter* Character) override;
};
