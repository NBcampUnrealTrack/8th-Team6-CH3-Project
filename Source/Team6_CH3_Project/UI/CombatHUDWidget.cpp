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
	//// 1. 4�� ���� ����� ����
	//if (WeaponSlot_1) WeaponSlot_1->SetRenderOpacity(SlotIndex == 1 ? 1.0f : 0.3f);
	//if (WeaponSlot_2) WeaponSlot_2->SetRenderOpacity(SlotIndex == 2 ? 1.0f : 0.3f);
	//if (WeaponSlot_3) WeaponSlot_3->SetRenderOpacity(SlotIndex == 3 ? 1.0f : 0.3f);
	//if (WeaponSlot_4) WeaponSlot_4->SetRenderOpacity(SlotIndex == 4 ? 1.0f : 0.3f);

	//// 2. ���� ���� �̹��� ������Ʈ
	//// Img_CurrentWeapon ������ �ְ�, ���� ���� �̹���(NewWeaponIcon)�� ���޵Ǿ��ٸ� ��ü!
	//if (Img_CurrentWeapon && NewWeaponIcon)
	//{
	//	Img_CurrentWeapon->SetBrushFromTexture(NewWeaponIcon);
	//}
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

void UCombatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();


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
}
