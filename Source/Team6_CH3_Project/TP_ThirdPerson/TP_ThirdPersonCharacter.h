#pragma once

#include "../Item/Weapons/SandboxWeaponBase.h"
#include "../Inventory/InventoryComponent.h"
#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TP_ThirdPersonCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);

UCLASS(config=Game, Blueprintable)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//
	// Weapon Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* WeaponMappingContext;
	// Weapon Input
	//

	//
	// Weapon Switch
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchMainWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchSubWeaponAction;
	// Weapon Switch
	// 

	//
	// Interact Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	// Interact Input
	//

	//
	// Inventory Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InventoryMappingContext;
	// Inventory Input
	//

public:
	ATP_ThirdPersonCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	//
	// Weapon Functions
	UFUNCTION(BlueprintCallable)
	void OnFire();

	UFUNCTION(BlueprintCallable)
	void OnFireStart();

	UFUNCTION(BlueprintCallable)
	void OnFireStop();

	UFUNCTION()
	void OnReload();

	FTimerHandle AutoFireTimer;
	// Weapon Functions
	//

	//
	// Weapon Switch Functions
	
	UFUNCTION(BlueprintCallable)
	void SwitchToMainWeapon();

	UFUNCTION(BlueprintCallable)
	void SwitchToSubWeapon();

	// Weapon Switch Functions
	//

	//
	// Interact Functions
	UFUNCTION()
	void OnInteract();
	// Interact Functions
	//

	//
	// Inventory Functions
	UFUNCTION(BlueprintCallable)
	void OnUseItem();
	// Inventory Functions
	//
protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	//
	// Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<ASandboxWeaponBase> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<ASandboxWeaponBase> MainWeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<ASandboxWeaponBase> SubWeaponSlot;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(ASandboxWeaponBase* Weapon);
	// Weapon
	//

	//
	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;
	// Inventory
	//

	//
	// Selected Slot
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 SelectedSlotIndex = 0;
	// Selected Slot
	//

	//
	// HUD Update

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateWeaponHUD();

	// HUD Update
	//
};

