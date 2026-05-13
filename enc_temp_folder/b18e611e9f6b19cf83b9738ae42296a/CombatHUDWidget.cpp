#include "CombatHUDWidget.h" // 헤더 파일 이름 변경됨!
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h" 
#include "Components/Image.h"

void UCombatHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar != nullptr)
	{
		float HealthPercentage = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
		HealthBar->SetPercent(HealthPercentage);

		// 체력이 30% 이하일 때 애니메이션 재생
		if (HealthPercentage <= 0.3f)
		{
			if (Anim_LowHealth != nullptr && !bIsLowHealthPlaying)
			{
				PlayAnimation(Anim_LowHealth, 0.0f, 0); // 0 = 무한 반복
				bIsLowHealthPlaying = true;
			}
		}
		// 체력이 30% 초과일 때 애니메이션 정지
		else
		{
			if (Anim_LowHealth != nullptr && bIsLowHealthPlaying)
			{
				StopAnimation(Anim_LowHealth);
				bIsLowHealthPlaying = false;
			}
		}
	}
}

void UCombatHUDWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoText != nullptr)
	{
		FString AmmoString = FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo);
		AmmoText->SetText(FText::FromString(AmmoString));
	}
}

void UCombatHUDWidget::SwitchWeaponSlot(int32 SlotIndex, UTexture2D* NewWeaponIcon)
{
	// 1. 4개 슬롯 투명도 조절 및 아이콘 변경
	auto SwapTexture = [](UImage* Img, UTexture2D* Tex)
		{
			if (!Img || !Tex) return;
			Img->SetBrushFromTexture(Tex, false);
			FSlateBrush Brush = Img->GetBrush();
			Brush.ImageSize = FVector2D(270.f, 230.f);
			Img->SetBrush(Brush);
		};
	if (WeaponSlot_1)
	{
		bool bActive = (SlotIndex == 1);
		WeaponSlot_1->SetRenderOpacity(bActive == 1 ? 1.0f : 0.3f);
		SwapTexture(WeaponSlot_1, bActive ? NewWeaponIcon : Icon_Rifle_Default);
	}
	if (WeaponSlot_2)
	{
		bool bActive = (SlotIndex == 2);
		WeaponSlot_2->SetRenderOpacity(SlotIndex == 2 ? 1.0f : 0.3f);
		SwapTexture(WeaponSlot_2, bActive ? NewWeaponIcon : Icon_Shotgun_Default);
	}
	if (WeaponSlot_3)
	{
		bool bActive = (SlotIndex == 3);
		WeaponSlot_3->SetRenderOpacity(bActive == 1 ? 1.0f : 0.3f);
		SwapTexture(WeaponSlot_3, bActive ? NewWeaponIcon : Icon_Pistol_Default);
	}
	// 2. 현재 무기 이미지 업데이트
	// Img_CurrentWeapon 위젯이 있고, 새로 넣을 이미지(NewWeaponIcon)가 전달되었다면 교체!
	if (Img_CurrentWeapon && NewWeaponIcon)
	{
		Img_CurrentWeapon->SetBrushFromTexture(NewWeaponIcon);
	}
}

void UCombatHUDWidget::SetMainWeaponSlotVisibility(bool bShowRifle, bool bShowShotgun)
{
	if (WeaponSlot_1)
		WeaponSlot_1->SetVisibility(bShowRifle ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (WeaponSlot_2)
		WeaponSlot_2->SetVisibility(bShowShotgun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UCombatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ATP_ThirdPersonCharacter* Character = Cast<ATP_ThirdPersonCharacter>(GetOwningPlayerPawn());

	UE_LOG(LogTemp, Warning, TEXT("Character: %s"),
		Character ? TEXT("valid") : TEXT("null"));
	if (!Character) return;

	CachedInventoryComp = Character->InventoryComponent;
	UE_LOG(LogTemp, Warning, TEXT("InventoryComp: %s"),
		CachedInventoryComp ? TEXT("valid") : TEXT("null"));
	if (!CachedInventoryComp) return;

	CachedInventoryComp->OnInventoryUpdated.AddDynamic(this, &UCombatHUDWidget::UpdateInventorySlot);

	UpdateInventorySlot();
}

void UCombatHUDWidget::UpdateInventorySlot()
{
	if (!InventorySlot_1 || !CachedInventoryComp) return;

	FInventorySlot& InvSlot = CachedInventoryComp->Slots[0];
	if (!InvSlot.bIsEmpty && InvSlot.ItemClass)
	{
		AItemBase* CDO = InvSlot.ItemClass->GetDefaultObject<AItemBase>();

		if (CDO && CDO->ItemIcon)
		{
			InventorySlot_1->SetBrushFromTexture(CDO->ItemIcon, true);
			InventorySlot_1->SetColorAndOpacity(FLinearColor::White);
			FSlateBrush Brush = InventorySlot_1->GetBrush();
			Brush.ImageSize = FVector2D(80.f, 100.f);
			InventorySlot_1->SetBrush(Brush);

			InventorySlot_1->SetVisibility(ESlateVisibility::Visible);
			InventorySlot_1->SetRenderOpacity(1.0f);
		}
	}
	else
	{
		InventorySlot_1->SetRenderOpacity(0.3f);
	}
}
