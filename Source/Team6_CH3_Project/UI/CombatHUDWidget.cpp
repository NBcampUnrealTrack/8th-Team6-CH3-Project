#include "CombatHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Player/PlayerStatComponent.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h" 
#include "TP_ThirdPerson/LunarAsylumCharacter.h"
#include "Components/Image.h"

void UCombatHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth, float Amount)
{
	if (HealthBar != nullptr)
	{
		float HealthPercentage = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
		HealthBar->SetPercent(HealthPercentage);

		if (HealthPercentage <= 0.3f)
		{
			if (Anim_LowHealth != nullptr && !bIsLowHealthPlaying)
			{
				PlayAnimation(Anim_LowHealth, 0.0f, 0);
				bIsLowHealthPlaying = true;
			}
		}
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
	// 1. 슬롯 투명도 조절 및 아이콘 변경
	auto SwapTexture = [](UImage* Img, UTexture2D* Tex)
		{
			if (!Img || !Tex) return;
			Img->SetBrushFromTexture(Tex, false);
			FSlateBrush Brush = Img->GetBrush();
			Brush.ImageSize = FVector2D(270.f, 230.f);
			Img->SetBrush(Brush);
		};
	
	if (Rifle)
	{
		bool bActive = (SlotIndex == 1);
		Rifle->SetRenderOpacity(bActive ? 1.0f : 0.3f);
		SwapTexture(Rifle, bActive ? NewWeaponIcon : Icon_Rifle_Default);
	}
	if (Shotgun)
	{
		bool bActive = (SlotIndex == 2);
		Shotgun->SetRenderOpacity(bActive ? 1.0f : 0.3f);
		SwapTexture(Shotgun, bActive ? NewWeaponIcon : Icon_Shotgun_Default);
	}
	if (Pistol)
	{
		bool bActive = (SlotIndex == 3);
		Pistol->SetRenderOpacity(bActive ? 1.0f : 0.3f);
		SwapTexture(Pistol, bActive ? NewWeaponIcon : Icon_Pistol_Default);
	}
	
	// 2. 현재 무기 이미지 업데이트
	if (Img_CurrentWeapon && NewWeaponIcon)
	{
		Img_CurrentWeapon->SetBrushFromTexture(NewWeaponIcon);
	}
}

void UCombatHUDWidget::SetIsAiming(bool bNewIsAiming)
{
	if (bNewIsAiming)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{ 
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCombatHUDWidget::SetMainWeaponSlotVisibility(bool bShowRifle, bool bShowShotgun)
{
	if (Rifle)
		Rifle->SetVisibility(bShowRifle ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (Shotgun)
		Shotgun->SetVisibility(bShowShotgun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UCombatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 무기 슬롯 초기 투명도
	if (Rifle) Rifle->SetRenderOpacity(0.3f);
	if (Shotgun) Shotgun->SetRenderOpacity(0.3f);
	if (Pistol) Pistol->SetRenderOpacity(0.3f);
	
	// 캐릭터 체력·조준 시스템
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		LunarAsylumCharacter = Cast<ALunarAsylumCharacter>(OwningPawn);
	}
	
	if (LunarAsylumCharacter)
	{
		LunarAsylumCharacter->PlayerStatComponent->OnHPChanged.AddDynamic(this, &UCombatHUDWidget::UpdateHealth);
		LunarAsylumCharacter->OnAimingChanged.AddDynamic(this, &UCombatHUDWidget::SetIsAiming);
		LunarAsylumCharacter->OnAimingChanged.Broadcast(false);
		
		float CurrentHP = LunarAsylumCharacter->PlayerStatComponent->PlayerStats.CurrentHP;
		float MaxHP = LunarAsylumCharacter->PlayerStatComponent->PlayerStats.MaxHP;
		LunarAsylumCharacter->PlayerStatComponent->OnHPChanged.Broadcast(CurrentHP, MaxHP, 0.f);
	}
	
	// 인벤토리 시스템
	ATP_ThirdPersonCharacter* Character = Cast<ATP_ThirdPersonCharacter>(GetOwningPlayerPawn());
	if (!Character) return;
	
	CachedInventoryComp = Character->InventoryComponent;
	if (!CachedInventoryComp) return;
	
	CachedInventoryComp->OnInventoryUpdated.AddDynamic(this, &UCombatHUDWidget::UpdateInventorySlot);
	UpdateInventorySlot();
}

void UCombatHUDWidget::UpdateInventorySlot()
{
	if (!InventorySlot_1 || !CachedInventoryComp) return;

	if (CachedInventoryComp->Slots.Num() == 0) return;

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
		InventorySlot_1->SetVisibility(ESlateVisibility::Hidden);
	}
}
