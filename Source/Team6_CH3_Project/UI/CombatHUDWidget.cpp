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
	// 1. 4개 슬롯 투명도 조절
	if (WeaponSlot_1) WeaponSlot_1->SetRenderOpacity(SlotIndex == 1 ? 1.0f : 0.3f);
	if (WeaponSlot_2) WeaponSlot_2->SetRenderOpacity(SlotIndex == 2 ? 1.0f : 0.3f);
	if (WeaponSlot_3) WeaponSlot_3->SetRenderOpacity(SlotIndex == 3 ? 1.0f : 0.3f);
	if (WeaponSlot_4) WeaponSlot_4->SetRenderOpacity(SlotIndex == 4 ? 1.0f : 0.3f);

	// 2. 현재 무기 이미지 업데이트
	// Img_CurrentWeapon 위젯이 있고, 새로 넣을 이미지(NewWeaponIcon)가 전달되었다면 교체!
	if (Img_CurrentWeapon && NewWeaponIcon)
	{
		Img_CurrentWeapon->SetBrushFromTexture(NewWeaponIcon);
	}
}