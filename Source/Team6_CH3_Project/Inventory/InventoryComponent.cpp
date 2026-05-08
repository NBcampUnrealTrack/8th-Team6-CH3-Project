// InventoryComponent.cpp

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Slots.SetNum(MaxSlots);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UInventoryComponent::AddItem(TSubclassOf<AItemBase> ItemClass, int32 Quantity)
{
	if (!ItemClass)return false;

	AItemBase* DefaultItem = ItemClass->GetDefaultObject<AItemBase>();

	if (DefaultItem->bIsStackable)
	{
		int32 ExistingSlot = FindItemSlot(ItemClass);
		if (ExistingSlot != -1)
		{
			if (Slots[ExistingSlot].Quantity + Quantity <= DefaultItem->MaxStackSize)
			{
				Slots[ExistingSlot].Quantity += Quantity;
				return true;
			}
		}
	}

	int32 EmptySlot = FindEmptySlot();
	if (EmptySlot == -1) return false;

	Slots[EmptySlot].ItemClass = ItemClass;
	Slots[EmptySlot].Quantity = Quantity;
	Slots[EmptySlot].bIsEmpty = false;

	OnInventoryUpdated.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Item added to slot %d"), EmptySlot);
	return true;
}

void UInventoryComponent::RemoveItem(int32 SlotIndex, int32 Quantity)
{
	if (!Slots.IsValidIndex(SlotIndex)) return;
	if (Slots[SlotIndex].bIsEmpty) return;

	Slots[SlotIndex].Quantity -= Quantity;

	if (Slots[SlotIndex].Quantity <= 0)
	{
		Slots[SlotIndex].ItemClass = nullptr;
		Slots[SlotIndex].Quantity = 0;
		Slots[SlotIndex].bIsEmpty = true;
	}

	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::IsFull() const
{
	return FindEmptySlot() == -1;
}

int32 UInventoryComponent::FindItemSlot(TSubclassOf<AItemBase> ItemClass) const
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (!Slots[i].bIsEmpty && Slots[i].ItemClass == ItemClass)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::FindEmptySlot() const
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].bIsEmpty)
		{
			return i;
		}
	}
	return -1;
}





void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

