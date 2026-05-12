
#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!SlotGrid || !SlotWidgetClass) return;

	SlotGrid->ClearChildren();

	for (int32 i = 0; i < SlotCount; i++)
	{
		UUserWidget* SlotWidget = CreateWidget<UUserWidget>(this, SlotWidgetClass);
		if (!SlotWidget) continue;

		const int32 Row = i / ColumnCount;
		const int32 Column = i % ColumnCount;

		UUniformGridSlot* GridSlot = SlotGrid->AddChildToUniformGrid(SlotWidget, Row, Column);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ATP_ThirdPersonCharacter* Character = Cast<ATP_ThirdPersonCharacter>(GetOwningPlayerPawn());

	if (!Character) return;

	CachedInventoryComp = Character->InventoryComponent;
	if (!CachedInventoryComp) return;

	CachedInventoryComp->OnInventoryUpdated.AddDynamic(
		this,
		&UInventoryWidget::UpdateInventoryUI
	);

	UpdateInventoryUI();
}

void UInventoryWidget::NativeDestruct()
{
	if (CachedInventoryComp)
	{
		CachedInventoryComp->OnInventoryUpdated.RemoveDynamic(
			this,
			&UInventoryWidget::UpdateInventoryUI
		);
	}

	Super::NativeDestruct();
}

void UInventoryWidget::UpdateInventoryUI()
{
	if (!SlotGrid || !CachedInventoryComp) return;

	TArray<UWidget*> Children = SlotGrid->GetAllChildren();

	for (int32 i = 0; i < Children.Num(); i++)
	{
		UUserWidget* SlotWidget = Cast<UUserWidget>(Children[i]);
		if (!SlotWidget) continue;

		if (i >= CachedInventoryComp->Slots.Num()) break;

		UFunction* UpdateSlotFunc = SlotWidget->FindFunction(FName("UpdateSlot"));
		if (UpdateSlotFunc)
		{
			struct FUpdateSlotParams
			{
				FInventorySlot SlotData;
				int32 SlotIndex = 0;
			} Params;

			Params.SlotData = CachedInventoryComp->Slots[i];
			Params.SlotIndex = i;

			SlotWidget->ProcessEvent(UpdateSlotFunc, &Params);
		}
	}
}