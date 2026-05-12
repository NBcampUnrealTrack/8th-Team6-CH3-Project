// InventoryWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventoryComponent;

UCLASS()
class TEAM6_CH3_PROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 SlotCount = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 ColumnCount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UUserWidget> SlotWidgetClass;

protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* SlotGrid;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateInventoryUI();

public:	
	
	UPROPERTY()
	UInventoryComponent* CachedInventoryComp;

};
