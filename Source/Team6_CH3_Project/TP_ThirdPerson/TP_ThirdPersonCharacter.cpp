// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../Item/Weapons/SandboxWeaponBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

//////////////////////////////////////////////////////////////////////////
// Input





void ATP_ThirdPersonCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
			Subsystem->AddMappingContext(InventoryMappingContext, 2);
		}
	}
}

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Look);

		// Weapon
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATP_ThirdPersonCharacter::OnFireStart);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATP_ThirdPersonCharacter::OnFireStop);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::OnReload);

		// Weapon Switch
		EnhancedInputComponent->BindAction(SwitchMainWeaponAction, ETriggerEvent::Started, this, &ATP_ThirdPersonCharacter::SwitchToMainWeapon);
		EnhancedInputComponent->BindAction(SwitchSubWeaponAction, ETriggerEvent::Started, this, &ATP_ThirdPersonCharacter::SwitchToSubWeapon);


		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::OnInteract);

		// Inventory
		//EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ATP_ThirdPersonCharacter::InInventoryToggle); IŰ
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::OnUseItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATP_ThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATP_ThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//
// Weapon
void ATP_ThirdPersonCharacter::EquipWeapon(ASandboxWeaponBase* Weapon)
{
	if (!Weapon) return;

	if (EquippedWeapon && EquippedWeapon != Weapon)
	{
		EquippedWeapon->SetActorHiddenInGame(true);
	}

	if (Weapon->SlotType == EItemSlotType::MainWeapon)
	{
		if (MainWeaponSlot)
		{
			MainWeaponSlot->Destroy();
		}
		MainWeaponSlot = Weapon;
	}
	else if (Weapon->SlotType == EItemSlotType::SubWeapon)
	{
		if (SubWeaponSlot)
		{
			SubWeaponSlot->Destroy();
		}
		SubWeaponSlot = Weapon;
	}

	EquippedWeapon = Weapon;
	Weapon->SetActorHiddenInGame(false);
	Weapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("ik_hand_gun"));
	OnWeaponChanged.Broadcast();
	UpdateWeaponHUD();
}

void ATP_ThirdPersonCharacter::OnFire()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Fire();
	}
}
void ATP_ThirdPersonCharacter::OnFireStart()
{
	if (EquippedWeapon)
	{
		GetWorldTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&ATP_ThirdPersonCharacter::OnFire,
			EquippedWeapon->GetRoF(),
			true
		);
		OnFire();
	}
	else
	{
	}
}
void ATP_ThirdPersonCharacter::OnFireStop()
{
	GetWorldTimerManager().ClearTimer(AutoFireTimer);
}

void ATP_ThirdPersonCharacter::OnReload()
{
	//if (EquippedWeapon)
	//{
	//	EquippedWeapon->Reload();
	//}
}
// Weapon
//

//
// Weapon Switch

void ATP_ThirdPersonCharacter::SwitchToMainWeapon()
{
	if (!MainWeaponSlot) return;

	if (SubWeaponSlot)
	{
		SubWeaponSlot->SetActorHiddenInGame(true);
	}

	EquippedWeapon = MainWeaponSlot;
	MainWeaponSlot->SetActorHiddenInGame(false);
	MainWeaponSlot->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("ik_hand_gun"));
	UpdateWeaponHUD();
}
void ATP_ThirdPersonCharacter::SwitchToSubWeapon()
{
	if (!SubWeaponSlot) return;

	if (MainWeaponSlot)
	{
		MainWeaponSlot->SetActorHiddenInGame(true);
	}

	EquippedWeapon = SubWeaponSlot;
	SubWeaponSlot->SetActorHiddenInGame(false);
	SubWeaponSlot->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("ik_hand_gun"));
	UpdateWeaponHUD();
}
// Weapon Switch
//

//
// Interact : F
void ATP_ThirdPersonCharacter::OnInteract()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AItemBase::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		// ����� �ٷ� ����
		ASandboxWeaponBase* Weapon = Cast<ASandboxWeaponBase>(Actor);
		if (Weapon)
		{
			Weapon->Interact(this);
			break;
		}

		// ���Ⱑ �ƴ� �������� �κ��丮��
		AItemBase* Item = Cast<AItemBase>(Actor);
		if (Item)
		{
			if (InventoryComponent->IsFull())
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
				break;
			}
			InventoryComponent->AddItem(Item->GetClass(), 1);
			Item->Destroy();
			break;
		}
	}
}
// Interact : F
//

//
// UseItem : E
void ATP_ThirdPersonCharacter::OnUseItem()
{
	if (!InventoryComponent) return;

	if (SelectedSlotIndex < 0 || SelectedSlotIndex >= InventoryComponent->Slots.Num()) return;

	FInventorySlot& Slot = InventoryComponent->Slots[SelectedSlotIndex];
	if (Slot.bIsEmpty || !Slot.ItemClass) return;

	AItemBase* CDO = Cast<AItemBase>(Slot.ItemClass->GetDefaultObject());
	if (!CDO) return;

	CDO->Use(this);

	InventoryComponent->RemoveItem(SelectedSlotIndex, 1);
}
// UseItem : E
//
