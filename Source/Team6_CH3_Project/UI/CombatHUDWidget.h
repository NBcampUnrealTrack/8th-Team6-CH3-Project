#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Inventory/InventoryComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "CombatHUDWidget.generated.h" // 이름 변경됨!


// 전방 선언
class UProgressBar;
class UTextBlock;
class UWidgetAnimation;
class UImage;
class UTexture2D;

UCLASS()
class TEAM6_CH3_PROJECT_API UCombatHUDWidget : public UUserWidget // UCombatHUDWidget으로 변경됨! (YOURPROJECTNAME은 본인 프로젝트명 유지)
{
	GENERATED_BODY()

public:
	// 팀원이 호출할 업데이트 함수
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float CurrentHealth, float MaxHealth, float Amount);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SwitchWeaponSlot(int32 SlotIndex, UTexture2D* NewWeaponIcon);

	UFUNCTION()
	void SetIsAiming(bool bNewIsAiming);

	UFUNCTION(BlueprintCallable)
	void SetMainWeaponSlotVisibility(bool bShowRifle, bool bShowShotgun);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Icons")
	UTexture2D* Icon_Rifle_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Icons")
	UTexture2D* Icon_Shotgun_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Icons")
	UTexture2D* Icon_Pistol_Default;

	UPROPERTY(meta = (BindWidget))
	UImage* InventorySlot_1;

	UPROPERTY()
	UInventoryComponent* CachedInventoryComp;

	UFUNCTION()
	void UpdateInventorySlot();

protected:
	// 블루프린트 UI 변수 연결 (UMG의 이름과 똑같아야 함)
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

	// 깜빡임 애니메이션 연결 (UMG 애니메이션 이름과 동일해야 함)
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Anim_LowHealth;

	UPROPERTY(meta = (BindWidget))
	UImage* Rifle;

	UPROPERTY(meta = (BindWidget))
	UImage* Shotgun;

	UPROPERTY(meta = (BindWidget))
	UImage* Pistol;


	// --- 🔫 현재 무기 이름 텍스트 ---
	UPROPERTY(meta = (BindWidget))
	UImage* Img_CurrentWeapon;

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	class ALunarAsylumCharacter* LunarAsylumCharacter = nullptr;

private:
	// 애니메이션 중복 재생 방지용 상태 변수
	bool bIsLowHealthPlaying = false;
};