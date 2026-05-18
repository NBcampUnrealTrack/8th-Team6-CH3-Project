// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ThirdPerson/LunarAsylumCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player\InputConfigData.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerStatComponent.h"
#include "../Inventory/InventoryComponent.h"
#include "InputActionValue.h"

ALunarAsylumCharacter::ALunarAsylumCharacter()
	: InputConfigData(nullptr)
	, DefaultMappingContext(nullptr)
	, bIsAiming(false)
	, bIsSprint(false)
	, CurrentAimSensitivity(AimSettings.NormalSensitivity)
	, CurrentEquipState(EEquipState::Unarmed)
	, CurrentActionState(EActionState::Idle)
	, CurrentWeapon(nullptr)
	, PrimaryWeapon(nullptr)
	, SecondaryWeapon(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetMesh(), FName(TEXT("spine_05")));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);

	PlayerStatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("StatComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ALunarAsylumCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentAimSensitivity = AimSettings.NormalSensitivity;
	GetCharacterMovement()->MaxWalkSpeed = MoveSettings.WalkSpeed;

	PlayerStatComponent->OnDeath.AddDynamic(this, &ALunarAsylumCharacter::OnDeath);
}

void ALunarAsylumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimZoom(DeltaTime);

	UpdateWeaponTransform(DeltaTime);

	UpdateWeaponIKWeight(DeltaTime);

	UpdateWeaponIKTransform();

	UpdateInteractionCheck();

	UpdatePlayerStateDebugMessage();
}

void ALunarAsylumCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		if (InputConfigData)
		{
			EnhancedInputComponent->BindAction(InputConfigData->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(InputConfigData->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			EnhancedInputComponent->BindAction(InputConfigData->MoveAction, ETriggerEvent::Triggered, this, &ALunarAsylumCharacter::Move);

			EnhancedInputComponent->BindAction(InputConfigData->LookAction, ETriggerEvent::Triggered, this, &ALunarAsylumCharacter::Look);

			EnhancedInputComponent->BindAction(InputConfigData->AimAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::StartAim);
			EnhancedInputComponent->BindAction(InputConfigData->AimAction, ETriggerEvent::Completed, this, &ALunarAsylumCharacter::StopAim);

			EnhancedInputComponent->BindAction(InputConfigData->SprintAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::StartSprint);
			EnhancedInputComponent->BindAction(InputConfigData->SprintAction, ETriggerEvent::Completed, this, &ALunarAsylumCharacter::StopSprint);

			EnhancedInputComponent->BindAction(InputConfigData->FireAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::StartFire);
			EnhancedInputComponent->BindAction(InputConfigData->FireAction, ETriggerEvent::Completed, this, &ALunarAsylumCharacter::StopFire);

			EnhancedInputComponent->BindAction(InputConfigData->PrimaryAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::PrimaryEquipToggle);

			EnhancedInputComponent->BindAction(InputConfigData->SecondaryAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::SecondaryEquipToggle);

			EnhancedInputComponent->BindAction(InputConfigData->UseItemAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::UseItem);

			EnhancedInputComponent->BindAction(InputConfigData->InteractAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::Interact);
		}
	}
}

void ALunarAsylumCharacter::NotifyControllerChanged()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

float ALunarAsylumCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!(CurrentActionState == EActionState::Dead || CurrentActionState == EActionState::HitReaction))
	{
		HitReaction();
		ApplyDamage(Damage);
	}

	return Damage;
}

void ALunarAsylumCharacter::ApplyDamage(float Amount)
{
	if (PlayerStatComponent)
	{
		PlayerStatComponent->ApplyDamage(Amount);
	}
}

void ALunarAsylumCharacter::AimSetting()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? MoveSettings.AimSpeed : MoveSettings.WalkSpeed;

	Camera->SetFieldOfView(bIsAiming ? AimSettings.AimFOV : AimSettings.DefaultFOV);

	CurrentAimSensitivity = bIsAiming ? AimSettings.AimSensitivity : AimSettings.NormalSensitivity;
}

void ALunarAsylumCharacter::UpdateAimZoom(float DeltaTime)
{
	float TargetArmLength = bIsAiming ? AimSettings.AimArmLength : AimSettings.DefaultArmLength;
	FVector TargetOffset = bIsAiming ? AimSettings.AimArmSocketOffset : AimSettings.DefaultArmSocketOffset;
	float InterpSpeed = bIsAiming ? AimSettings.AimInterpSpeed : AimSettings.DefaultInterpSpeed;

	CameraArm->TargetArmLength = FMath::FInterpTo(CameraArm->TargetArmLength, TargetArmLength, DeltaTime, InterpSpeed);
	CameraArm->SocketOffset = FMath::VInterpTo(CameraArm->SocketOffset, TargetOffset, DeltaTime, InterpSpeed);
}

void ALunarAsylumCharacter::StartAim()
{
	if (CurrentEquipState == EEquipState::Unarmed) return;

	bIsAiming = true;
	AimSetting();
	OnAimingChanged.Broadcast(bIsAiming);
}

void ALunarAsylumCharacter::StopAim()
{
	bIsAiming = false;
	AimSetting();
	OnAimingChanged.Broadcast(bIsAiming);
}

void ALunarAsylumCharacter::OnFire()
{
	if (CurrentWeapon && CurrentWeapon->CanAttack() && CurrentEquipState != EEquipState::Unarmed && CurrentActionState == EActionState::Idle)
	{
		CurrentWeapon->Fire();

		PlayAnimMontage(CurrentWeapon->CharacterAnimMontages.Fire);
		if (APlayerCameraManager* CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
		{
			CamManager->StartCameraShake(CurrentWeapon->CameraShakeClass, 1.0f);
		}

		if (CurrentWeapon->MuzzleEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(CurrentWeapon->MuzzleEffect, CurrentWeapon->Mesh, FName(TEXT("Socket_Muzzle")), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		}
	}
}

void ALunarAsylumCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &ALunarAsylumCharacter::OnFire, CurrentWeapon->GetRoF(), true);
		OnFire();
	}
}

void ALunarAsylumCharacter::StopFire()
{
	GetWorldTimerManager().ClearTimer(AutoFireTimer);
}

void ALunarAsylumCharacter::StartSprint()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSettings.SprintSpeed;
}

void ALunarAsylumCharacter::StopSprint()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = MoveSettings.WalkSpeed;
}

void ALunarAsylumCharacter::Interact()
{
	//TArray<AActor*> OverlappingActors;
	//GetOverlappingActors(OverlappingActors, AItemBase::StaticClass());

	//for (AActor* Actor : OverlappingActors)
	//{
		ASandboxWeaponBase* Weapon = Cast<ASandboxWeaponBase>(TargetItem);
		if (Weapon)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASandboxWeaponBase* NewWeapon = GetWorld()->SpawnActor<ASandboxWeaponBase>(Weapon->GetClass(), GetActorLocation(), GetActorRotation(), SpawnParams);

			if (NewWeapon)
			{
				NewWeapon->SetActorEnableCollision(false);

				if (NewWeapon->SlotType == EItemSlotType::MainWeapon && !PrimaryWeapon)
				{
					PrimaryWeapon = NewWeapon;
					InternalAttachWeapon(PrimaryWeapon, TEXT("PrimaryWeaponSocket"), PrimaryWeapon->HolsterOffset);
					UE_LOG(LogTemp, Log, TEXT("주무기 부착"));
				}
				else if (NewWeapon->SlotType == EItemSlotType::SubWeapon && !SecondaryWeapon)
				{
					SecondaryWeapon = NewWeapon;
					InternalAttachWeapon(SecondaryWeapon, TEXT("SecondaryWeaponSocket"), SecondaryWeapon->HolsterOffset);
					UE_LOG(LogTemp, Log, TEXT("부무기 부착"));
				}
				else
				{
					InventoryComponent->AddItem(NewWeapon->GetClass(), 1);
					NewWeapon->Destroy();
				}
			}

			Weapon->Destroy();
			//break;
		//}
	}


	//ASandboxWeaponBase* Weapon = Cast<ASandboxWeaponBase>(Actor);
	//if (Weapon)
	//{
	//	Weapon->Interact(this);
	//	break;
	//}
	//AItemBase* Item = Cast<AItemBase>(Actor);
	//if (Item)
	//{
	//	if (!InventoryComponent->Slots[0].bIsEmpty)
	//	{
	//		InventoryComponent->RemoveItem(0, 1);
	//	}
	//	InventoryComponent->AddItem(Item->GetClass(), 1);
	//	Item->Destroy();
	//	break;
	//}
//}


}

void ALunarAsylumCharacter::UseItem()
{
	//if (!InventoryComponent) return;

	//if (SelectedSlotIndex < 0 || SelectedSlotIndex >= InventoryComponent->Slots.Num()) return;

	//FInventorySlot& Slot = InventoryComponent->Slots[SelectedSlotIndex];
	//if (Slot.bIsEmpty || !Slot.ItemClass) return;

	//AItemBase* CDO = Cast<AItemBase>(Slot.ItemClass->GetDefaultObject());
	//if (!CDO) return;

	//CDO->Use(this);

	//InventoryComponent->RemoveItem(SelectedSlotIndex, 1);

	if (!InventoryComponent) return;

	if (SelectedSlotIndex < 0 || SelectedSlotIndex >= InventoryComponent->Slots.Num()) return;

	FInventorySlot& Slot = InventoryComponent->Slots[SelectedSlotIndex];
	if (Slot.bIsEmpty || !Slot.ItemClass) return;

	AItemBase* ItemCDO = Slot.ItemClass->GetDefaultObject<AItemBase>();

	if (ItemCDO->ItemType == EItemType::Weapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		ASandboxWeaponBase* NewWeapon = GetWorld()->SpawnActor<ASandboxWeaponBase>(
			Slot.ItemClass,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParams
		);

		if (NewWeapon)
		{
			EquipWeapon(NewWeapon);
			InventoryComponent->RemoveItem(SelectedSlotIndex, 1);
		}
	}
	else
	{
		ItemCDO->Use(this);
		InventoryComponent->RemoveItem(SelectedSlotIndex, 1);
	}

}

void ALunarAsylumCharacter::EquipWeapon(ASandboxWeaponBase* Weapon)
{
	if (!Weapon) return;

	if (Weapon->SlotType == EItemSlotType::MainWeapon)
	{
		if (PrimaryWeapon)
		{
			PrimaryWeapon->Destroy();
		}
		PrimaryWeapon = Weapon;
	}
	else if (Weapon->SlotType == EItemSlotType::SubWeapon)
	{
		if (SecondaryWeapon)
		{
			SecondaryWeapon->Destroy();
		}
		SecondaryWeapon = Weapon;
	}

	CurrentWeapon = Weapon;

	if (CurrentWeapon->WeaponType == EWeaponType::Shotgun || CurrentWeapon->WeaponType == EWeaponType::Rifle)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("PrimaryWeaponSocket"));
	}
	else
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("SecondaryWeaponSocket"));
	}
}

void ALunarAsylumCharacter::ANAttachWeapon()
{
	if (CurrentWeapon && GetMesh())
	{
		FTransform WorldTransform = CurrentWeapon->Mesh->GetComponentTransform();
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		CurrentWeapon->Mesh->SetWorldTransform(WorldTransform);
		TargetWeaponTransform = CurrentWeapon->EquipOffset;
		bIsInterpWeaponTransform = true;
		//bIsIKAlpha = true;
		//TargetIKAlpha = 1.f;
	}

}

void ALunarAsylumCharacter::ANHolsterWeapon()
{

	if (CurrentWeapon && GetMesh())
	{
		FName TargetSocket = (CurrentWeapon->WeaponType == EWeaponType::Pistol) ? TEXT("SecondaryWeaponSocket") : TEXT("PrimaryWeaponSocket");
		FTransform WorldTransform = CurrentWeapon->Mesh->GetComponentTransform();

		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TargetSocket);
		CurrentWeapon->Mesh->SetWorldTransform(WorldTransform);
		TargetWeaponTransform = CurrentWeapon->HolsterOffset;
		bIsInterpWeaponTransform = true;
		//bIsIKAlpha = true;
		//TargetIKAlpha = 0.f;
	}
}

void ALunarAsylumCharacter::InternalAttachWeapon(ASandboxWeaponBase* Weapon, FName SocketName, const FTransform& Offset)
{
	if (!Weapon || !Weapon->Mesh || !GetMesh()) return;

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

	Weapon->Mesh->SetRelativeLocation(Offset.GetLocation());
	Weapon->Mesh->SetRelativeRotation(Offset.GetRotation());
}

void ALunarAsylumCharacter::UpdateWeaponTransform(float DeltaTime)
{
	if (!bIsInterpWeaponTransform || !CurrentWeapon || !CurrentWeapon->Mesh)
	{
		return;
	}

	FVector CurrentLoc = CurrentWeapon->Mesh->GetRelativeLocation();
	FRotator CurrentRot = CurrentWeapon->Mesh->GetRelativeRotation();

	FVector TargetLoc = TargetWeaponTransform.GetLocation();
	FRotator TargetRot = TargetWeaponTransform.GetRotation().Rotator();

	FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, WeaponInterpSpeed);
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, WeaponInterpSpeed);

	CurrentWeapon->Mesh->SetRelativeLocation(NewLoc);
	CurrentWeapon->Mesh->SetRelativeRotation(NewRot);

	if (NewLoc.Equals(TargetLoc, 0.1f) && NewRot.Equals(TargetRot, 0.1f))
	{
		bIsInterpWeaponTransform = false;
		CurrentWeapon->Mesh->SetRelativeLocation(TargetLoc);
		CurrentWeapon->Mesh->SetRelativeRotation(TargetRot);
	}
}

void ALunarAsylumCharacter::UpdateWeaponIKTransform()
{
	if (CurrentEquipState != EEquipState::Unarmed && CurrentWeapon)
	{
		FTransform WeaponSocketTransform = CurrentWeapon->Mesh->GetSocketTransform(FName(TEXT("Socket_LeftHand")));
		FVector WeaponIKLocation;
		FRotator WeaponIKRotation;

		GetMesh()->TransformToBoneSpace(FName(TEXT("hand_r")), WeaponSocketTransform.GetLocation(), WeaponSocketTransform.GetRotation().Rotator(), WeaponIKLocation, WeaponIKRotation);

		WeaponEffector.SetLocation(WeaponIKLocation);
		WeaponEffector.SetRotation(WeaponIKRotation.Quaternion());
	}
}

void ALunarAsylumCharacter::UpdateWeaponIKWeight(float DeltaTime)
{
	LeftHandIKAlpha = FMath::FInterpTo(LeftHandIKAlpha, TargetIKAlpha, DeltaTime, IKInterpSpeed);

	if (FMath::IsNearlyEqual(LeftHandIKAlpha, TargetIKAlpha, 0.01f))
	{
		bIsIKAlpha = false;
		LeftHandIKAlpha = TargetIKAlpha;
	}
}

void ALunarAsylumCharacter::OnEquipMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		TargetIKAlpha = 1.f;
		bIsIKAlpha = true;
	}
}

void ALunarAsylumCharacter::UpdatePlayerStateDebugMessage()
{
	// 캐릭터 상태 출력 확인용//
	FString ActionStateString = UEnum::GetValueAsString(CurrentActionState);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, ActionStateString);
	FString EquipStateString = UEnum::GetValueAsString(CurrentEquipState);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, EquipStateString);
}

void ALunarAsylumCharacter::UpdateInteractionCheck()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	// 화면 크기 가져오기
	int32 ScreenWidth, ScreenHeight;
	PC->GetViewportSize(ScreenWidth, ScreenHeight);

	// 화면 정중앙 2D 좌표 계산
	FVector2D ScreenCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);

	FVector WorldLocation, WorldDirection;
	// 3. 2D 화면 중앙을 3D 월드 시작점과 방향 벡터로 변환


	if (PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
	{
		// 변환 성공 시, 기존 시작/끝 계산 방식 교체
		FVector StartLocation = WorldLocation;
		FVector End = StartLocation + (WorldDirection * InteractionDistance);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionShape SweepSphere = FCollisionShape::MakeSphere(20.f);

		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			End,
			FQuat::Identity,      
			ECC_GameTraceChannel2, 
			SweepSphere,           
			Params
		);

		// 디버그 출력
		//DrawDebugSphere(GetWorld(), StartLocation, 20.f, 12, FColor::Red, false, 0.1f);
		DrawDebugSphere(GetWorld(), HitResult.Location, 20.f, 12, bHit ? FColor::Green : FColor::Red, false, 0.1f);

		if (bHit)
		{
			//UE_LOG(LogTemp, Log, TEXT("아이템 : %s"), *HitResult.GetActor()->GetName());

			AItemBase* InteractionItem = Cast<AItemBase>(HitResult.GetActor());
			if (InteractionItem && TargetItem != InteractionItem)
			{
				TargetItem = InteractionItem;
				OnTargetItemChanged.Broadcast(TargetItem->GetActorLabel()); 
				//GEngine->AddOnScreenDebugMessage(-1, 9.f, FColor::Cyan, FString::Printf(TEXT("아이템 정보 : %s"), *TargetItem->GetActorNameOrLabel()));
			}
		}
		else
		{
			if (TargetItem != nullptr)
			{
				TargetItem = nullptr;
				OnTargetItemChanged.Broadcast(TEXT(""));
			}
		}


	}


	//FVector StartLocation = Camera->GetComponentLocation() + Camera->GetComponentRotation().Vector() * 175.f;

	//FVector End = StartLocation + Camera->GetComponentRotation().Vector() * InteractionDistance;

	//FHitResult HitResult;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);
	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	HitResult,
	//	StartLocation,
	//	End,
	//	ECC_GameTraceChannel2,
	//	Params
	//);

	//// 연출용 트레이스 : 디버그 출력
	//DrawDebugLine(
	//	GetWorld(),
	//	StartLocation,
	//	End,
	//	FColor::Blue,
	//	false,
	//	5.f,
	//	0,
	//	2.0f
	//);

}

void ALunarAsylumCharacter::PrimaryEquipToggle()
{
	if (PrimaryWeapon)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CurrentEquipState == EEquipState::Unarmed && CurrentActionState == EActionState::Idle)
		{
			FOnMontageEnded EndDelegate;
			CurrentWeapon = PrimaryWeapon;
			AnimInstance->Montage_Play(CurrentWeapon->CharacterAnimMontages.Equip);
			//PlayAnimMontage(CurrentWeapon->CharacterAnimMontages.Equip);

			EndDelegate.BindUObject(this, &ALunarAsylumCharacter::OnEquipMontageEnd);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentWeapon->CharacterAnimMontages.Equip);
			return;
		}

		if (CurrentEquipState == EEquipState::Primary && CurrentActionState == EActionState::Idle)
		{
			PlayAnimMontage(CurrentWeapon->CharacterAnimMontages.Holster);
			LeftHandIKAlpha = 0.f;
			TargetIKAlpha = 0.f;
			bIsIKAlpha = false;
		}
	}
}

void ALunarAsylumCharacter::SecondaryEquipToggle()
{
	if (SecondaryWeapon)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CurrentEquipState == EEquipState::Unarmed && CurrentActionState == EActionState::Idle)
		{
			FOnMontageEnded EndDelegate;
			CurrentWeapon = SecondaryWeapon;
			AnimInstance->Montage_Play(CurrentWeapon->CharacterAnimMontages.Equip);
			//PlayAnimMontage(CurrentWeapon->CharacterAnimMontages.Equip);
			EndDelegate.BindUObject(this, &ALunarAsylumCharacter::OnEquipMontageEnd);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentWeapon->CharacterAnimMontages.Equip);
			return;
		}
		if (CurrentEquipState == EEquipState::Secondary && CurrentActionState == EActionState::Idle)
		{
			PlayAnimMontage(CurrentWeapon->CharacterAnimMontages.Holster);
			LeftHandIKAlpha = 0.f;
			TargetIKAlpha = 0.f;
			bIsIKAlpha = false;
		}
	}
}

void ALunarAsylumCharacter::HitReaction()
{
	//if (CurrentActionState == EActionState::Dead) return;
	//StopAnimMontage();

	if (AM_HitReaction)
	{
		PlayAnimMontage(AM_HitReaction);
	}
}

void ALunarAsylumCharacter::OnDeath()
{

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		StopAnimMontage();
		DisableInput(PlayerController);

		if (AM_Death)
		{
			PlayAnimMontage(AM_Death);
		}
	}
}

void ALunarAsylumCharacter::SetActionState(EActionState NewState)
{
	if (CurrentActionState != NewState)
	{
		CurrentActionState = NewState;
		OnActionStateChanged.Broadcast(NewState);
	}
}

void ALunarAsylumCharacter::SetEquipState(EEquipState NewState)
{
	if (CurrentEquipState != NewState)
	{
		CurrentEquipState = NewState;
		OnEquipStateChanged.Broadcast(CurrentEquipState);
	}
}

void ALunarAsylumCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALunarAsylumCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * CurrentAimSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * CurrentAimSensitivity);
	}
}
