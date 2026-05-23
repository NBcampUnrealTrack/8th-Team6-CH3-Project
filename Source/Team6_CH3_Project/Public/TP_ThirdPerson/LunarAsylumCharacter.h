#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectTypes.h"
#include "LunarAsylumCharacter.generated.h"

// 전방 선언 (Forward Declarations)
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputConfigData;
class UPlayerStatComponent;
class UInventoryComponent;
class UAnimMontage;
class ASandboxWeaponBase;
class AItemBase;
class USpotLightComponent;
struct FInputActionInstance;

//-----------------------------------------------------------------------------
// DELEGATES
//-----------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetItemChangedSignature, const FString&, ItemName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAcquiredSignature, const FString&, ItemAcquiredName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionProgressChanged, float, NewPercent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipStateChanged, EEquipState, EquipState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimingChanged, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionStateChanged, EActionState, ActionState);


UCLASS()
class TEAM6_CH3_PROJECT_API ALunarAsylumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALunarAsylumCharacter();

	//-----------------------------------------------------------------------------
	// COMPONENTS
	//-----------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	UPlayerStatComponent* PlayerStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	USpotLightComponent* Torch;

	//-----------------------------------------------------------------------------
	// INPUT CONFIG
	//-----------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputConfigData* InputConfigData;

	//-----------------------------------------------------------------------------
	// MOVEMENT & AIM SETTINGS
	//-----------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Settings|Movement")
	FMovementSetting MoveSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Settings|Aim")
	FAimSetting AimSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Settings|Aim")
	float CurrentAimSensitivity;

	//-----------------------------------------------------------------------------
	// ENUM STATES
	//-----------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|State")
	EEquipState CurrentEquipState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|State")
	EActionState CurrentActionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugPlayerState = false;

	//-----------------------------------------------------------------------------
	// WEAPONS & COMBAT
	//-----------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat")
	ASandboxWeaponBase* PrimaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat")
	ASandboxWeaponBase* SecondaryWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Combat")
	ASandboxWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat|Animation")
	UAnimMontage* AMHitReactionUnarmed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat|Animation")
	UAnimMontage* AMHitReactionArmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat|Animation")
	UAnimMontage* AM_Death;

	bool bWantsToFire = false;

	//-----------------------------------------------------------------------------
	// WEAPONS TRANSFORMS & IK SYSTEM
	//-----------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat|IK")
	float WeaponInterpSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Combat|IK")
	float LeftHandIKAlpha = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Combat|IK")
	float IKInterpSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Combat|IK")
	FTransform WeaponEffector;

	bool bIsInterpWeaponTransform = false;

	FTransform TargetWeaponTransform;

	float TargetIKAlpha = 0.f;

	bool bIsIKAlpha = false;

	//-----------------------------------------------------------------------------
	// INVENTORY & INTERACTION
	//-----------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Inventory")
	int32 SelectedSlotIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Interaction")
	AItemBase* TargetItem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Interaction")
	float InteractionDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Interaction")
	float InteractHoldTimeDuration = 0.5f;

	float InteractionProgressPercent = 0.f;

	float InteractionCurrentTime = 0.f;

	//-----------------------------------------------------------------------------
	// DELEGATE INSTANCES
	//-----------------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "Player|Delegates")
	FEquipStateChanged OnEquipStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Delegates")
	FActionStateChanged OnActionStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Delegates")
	FAimingChanged OnAimingChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Delegates")
	FOnTargetItemChangedSignature OnTargetItemChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Delegates")
	FOnItemAcquiredSignature OnItemAcquired;

	UPROPERTY(BlueprintAssignable, Category = "Player|Delegates")
	FOnInteractionProgressChanged OnInteractionProgressChanged;

	//-----------------------------------------------------------------------------
	// PUBLIC METHODS
	//-----------------------------------------------------------------------------
	void SetActionState(EActionState NewState);
	void SetEquipState(EEquipState NewState);

	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	void ANAttachWeapon();

	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	void ANHolsterWeapon();

	void InternalAttachWeapon(ASandboxWeaponBase* Weapon, FName SocketName, const FTransform& Offset);
	void UpdateWeaponTransform(float DeltaTime);
	void UpdateWeaponIKTransform();
	void UpdateWeaponIKWeight(float DeltaTime);
	void OnEquipMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	void UpdatePlayerStateDebugMessage();

protected:
	//-----------------------------------------------------------------------------
	// ENGINE OVERRIDES
	//-----------------------------------------------------------------------------
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//-----------------------------------------------------------------------------
	// INPUT & MOVEMENT METHODS
	//-----------------------------------------------------------------------------
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void ToggleTorch();

	//-----------------------------------------------------------------------------
	// COMBAT & AIM METHODS
	//-----------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Player|Input|Actions")
	void StartAim();

	UFUNCTION(BlueprintCallable, Category = "Player|Input|Actions")
	void StopAim();

	void AimSetting();
	void UpdateAimZoom(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Player|Input|Actions")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player|Input|Actions")
	void StopFire();

	void PrimaryEquipToggle();
	void SecondaryEquipToggle();

	void DropPrimaryWeapon();
	void DropSecondaryWeapon();
	void DropWeapon(EEquipState EquipState);

	//-----------------------------------------------------------------------------
	// INTERACTION 
	//-----------------------------------------------------------------------------
	void Interaction();

	void UpdateInteractionCheck();

	void OnInteractStarted();

	void OnInteractTriggered(const FInputActionInstance& Instance);

	void OnInteractCanceled();

	//-----------------------------------------------------------------------------
	// DAMAGE
	//-----------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Player|Damage")
	void ApplyDamage(float Amount);

	void HitReaction();

	UFUNCTION()
	void OnDeath();
};