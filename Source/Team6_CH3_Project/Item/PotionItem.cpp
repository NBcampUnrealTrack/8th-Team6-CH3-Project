#include "PotionItem.h"
#include "GameFramework/Character.h"
#include "Player/PlayerStatComponent.h"

APotionItem::APotionItem()
{
	ItemType = EItemType::Consumable;
	bIsStackable = true;
	MaxStackSize = 10;
	ItemName = FName("Potion");
}

void APotionItem::Use(ACharacter* Character)
{
	if (!Character) return;

	UPlayerStatComponent* StatComp = Character->FindComponentByClass<UPlayerStatComponent>();
	if (StatComp)
	{
		StatComp->HealHP(HealAmount);
	}
}
