// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ThirdPerson/LunarAsylumCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Item/ItemBase.h"
#include "../Item/Weapons/SandboxWeaponBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

#include "InputAction.h"
#include "Components\SpotLightComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player\InputConfigData.h"
#include "InputTriggers.h"
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

	Torch = CreateDefaultSubobject<USpotLightComponent>(TEXT("Torch"));
	Torch->SetupAttachment(Camera);
	Torch->SetRelativeLocation(FVector(350.f, 0.f, 0.f));

	Torch->SetIntensityUnits(ELightUnits::Lumens);

	Torch->Intensity = 100.0f;
	Torch->AttenuationRadius = 2000.0f;

	Torch->InnerConeAngle = 10.0f;
	Torch->OuterConeAngle = 25.0f;

	CurrentAimSensitivity = AimSettings.NormalSensitivity;

	CameraArm->TargetArmLength = AimSettings.DefaultArmLength;

	CameraArm->SocketOffset = AimSettings.DefaultArmSocketOffset;

	PlayerStatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("StatComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ALunarAsylumCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InputConfigData && InputConfigData->InteractAction)
	{
		for (const UInputTrigger* Trigger : InputConfigData->InteractAction->Triggers)
		{
			if (const UInputTriggerHold* HoldTrigger = Cast<UInputTriggerHold>(Trigger))
			{
				InteractHoldTimeDuration = HoldTrigger->HoldTimeThreshold;
				break;
			}
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = MoveSettings.WalkSpeed;

	Torch->SetVisibility(false);

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

			EnhancedInputComponent->BindAction(InputConfigData->TorchAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::ToggleTorch);

			EnhancedInputComponent->BindAction(InputConfigData->InteractAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::OnInteractStarted);
			EnhancedInputComponent->BindAction(InputConfigData->InteractAction, ETriggerEvent::Ongoing, this, &ALunarAsylumCharacter::OnInteractOngoing);
			EnhancedInputComponent->BindAction(InputConfigData->InteractAction, ETriggerEvent::Triggered, this, &ALunarAsylumCharacter::OnInteractTriggered);
			EnhancedInputComponent->BindAction(InputConfigData->InteractAction, ETriggerEvent::Completed, this, &ALunarAsylumCharacter::OnInteractCanceled);
			EnhancedInputComponent->BindAction(InputConfigData->InteractAction, ETriggerEvent::Canceled, this, &ALunarAsylumCharacter::OnInteractCanceled);

			EnhancedInputComponent->BindAction(InputConfigData->PrimaryDropAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::DropPrimaryWeapon);

			EnhancedInputComponent->BindAction(InputConfigData->SecondaryDropAction, ETriggerEvent::Started, this, &ALunarAsylumCharacter::DropSecondaryWeapon);
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
	CurrentAimSensitivity = bIsAiming ? AimSettings.AimSensitivity : AimSettings.NormalSensitivity;
}

void ALunarAsylumCharacter::UpdateAimZoom(float DeltaTime)
{
	float TargetArmLength = bIsAiming ? AimSettings.AimArmLength : AimSettings.DefaultArmLength;
	FVector TargetOffset = bIsAiming ? AimSettings.AimArmSocketOffset : AimSettings.DefaultArmSocketOffset;
	float InterpSpeed = bIsAiming ? AimSettings.AimInterpSpeed : AimSettings.DefaultInterpSpeed;
	float TargetFOV = bIsAiming ? AimSettings.AimFOV : AimSettings.DefaultFOV;

	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, InterpSpeed));
	CameraArm->TargetArmLength = FMath::FInterpTo(CameraArm->TargetArmLength, TargetArmLength, DeltaTime, InterpSpeed);
	CameraArm->SocketOffset = FMath::VInterpTo(CameraArm->SocketOffset, TargetOffset, DeltaTime, InterpSpeed);
}

void ALunarAsylumCharacter::StartAim()
{
	if (CurrentEquipState == EEquipState::Unarmed)
	{
		return;
	}

	if (bIsSprint)
	{
		StopSprint();
	}

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

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (APlayerCameraManager* CamManager = PC->PlayerCameraManager)
			{
				CamManager->StartCameraShake(CurrentWeapon->CameraShakeClass, 1.0f);
			}
		}

		if (CurrentWeapon->MuzzleEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(CurrentWeapon->MuzzleEffect, CurrentWeapon->Mesh, FName(TEXT("Socket_Muzzle")), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		}
	}
}

void ALunarAsylumCharacter::StartFire()
{
	if (!CurrentWeapon || CurrentEquipState == EEquipState::Unarmed)
	{
		return;
	}

	if (CurrentActionState == EActionState::Idle)
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
	if (bIsAiming || CurrentActionState == EActionState::Firing)
	{
		return;
	}

	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSettings.SprintSpeed;
}

void ALunarAsylumCharacter::StopSprint()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = MoveSettings.WalkSpeed;
}

void ALunarAsylumCharacter::ToggleTorch()
{
	if (Torch)
	{
		Torch->ToggleVisibility();
	}
}

void ALunarAsylumCharacter::Interaction()
{
	if (!TargetItem)
	{
		return;
	}

	ASandboxWeaponBase* GroundWeapon = Cast<ASandboxWeaponBase>(TargetItem);
	if (GroundWeapon)
	{
		bool bHasPrimarySameType = PrimaryWeapon && (PrimaryWeapon->GetClass() == GroundWeapon->GetClass());
		bool bHasSecondarySameType = SecondaryWeapon && (SecondaryWeapon->GetClass() == GroundWeapon->GetClass());

		if (bHasPrimarySameType || bHasSecondarySameType)
		{
			ASandboxWeaponBase* MyOwnedWeapon = bHasPrimarySameType ? PrimaryWeapon : SecondaryWeapon;

			if (MyOwnedWeapon)
			{
				MyOwnedWeapon->MaxAmmoAdd();

				FString AmmoMessage = FString::Printf(TEXT("%s 탄약 충전!"), *GroundWeapon->ItemName.ToString());
				OnItemAcquired.Broadcast(AmmoMessage);

				TargetItem->Destroy();
				TargetItem = nullptr;
				OnTargetItemChanged.Broadcast(TEXT(""));
			}
			return;
		}

		bool bCanEquipAsMain = (GroundWeapon->SlotType == EItemSlotType::MainWeapon && !PrimaryWeapon);
		bool bCanEquipAsSub = (GroundWeapon->SlotType == EItemSlotType::SubWeapon && !SecondaryWeapon);

		if (bCanEquipAsMain || bCanEquipAsSub)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ASandboxWeaponBase* NewWeapon = GetWorld()->SpawnActor<ASandboxWeaponBase>(
				GroundWeapon->GetClass(),
				GetActorLocation(),
				GetActorRotation(),
				SpawnParams
			);

			NewWeapon->SetCurrentAmmo(GroundWeapon->GetCurrentAmmo());
			UE_LOG(LogTemp, Log, TEXT("Current Ammo : %d"), NewWeapon->GetCurrentAmmo());

			if (NewWeapon)
			{
				NewWeapon->Mesh->SetSimulatePhysics(false);
				NewWeapon->Mesh->SetCollisionProfileName(TEXT("NoCollision"));
				NewWeapon->SetActorEnableCollision(false);

				if (NewWeapon->GetRootComponent())
				{
					NewWeapon->Mesh->AttachToComponent(NewWeapon->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				}

				NewWeapon->Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
				NewWeapon->Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
				NewWeapon->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);

				FString AcquiredMessage = FString::Printf(TEXT("%s 장착 완료"), *NewWeapon->ItemName.ToString());
				OnItemAcquired.Broadcast(AcquiredMessage);

				if (NewWeapon->SlotType == EItemSlotType::MainWeapon)
				{
					PrimaryWeapon = NewWeapon;
					InternalAttachWeapon(PrimaryWeapon, TEXT("PrimaryWeaponSocket"), PrimaryWeapon->HolsterOffset);
				}
				else
				{
					SecondaryWeapon = NewWeapon;
					InternalAttachWeapon(SecondaryWeapon, TEXT("SecondaryWeaponSocket"), SecondaryWeapon->HolsterOffset);
				}

				GroundWeapon->Destroy();
				TargetItem = nullptr;
				OnTargetItemChanged.Broadcast(TEXT(""));
			}
		}
		else
		{
			OnItemAcquired.Broadcast(TEXT("해당 무기 슬롯이 가득 찼습니다!"));
		}
	}
}

void ALunarAsylumCharacter::ANAttachWeapon()
{
	if (CurrentWeapon && GetMesh())
	{
		FTransform WorldTransform = CurrentWeapon->GetActorTransform();

		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));

		CurrentWeapon->SetActorTransform(WorldTransform);

		TargetWeaponTransform = CurrentWeapon->EquipOffset;

		bIsInterpWeaponTransform = true;
	}
}

void ALunarAsylumCharacter::ANHolsterWeapon()
{

	if (CurrentWeapon && GetMesh())
	{
		FName TargetSocket = (CurrentWeapon->WeaponType == EWeaponType::Pistol) ? TEXT("SecondaryWeaponSocket") : TEXT("PrimaryWeaponSocket");

		FTransform WorldTransform = CurrentWeapon->GetActorTransform();
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TargetSocket);

		CurrentWeapon->SetActorTransform(WorldTransform);

		TargetWeaponTransform = CurrentWeapon->HolsterOffset;

		bIsInterpWeaponTransform = true;
	}
}

void ALunarAsylumCharacter::InternalAttachWeapon(ASandboxWeaponBase* Weapon, FName SocketName, const FTransform& Offset)
{
	if (!Weapon || !Weapon->Mesh || !GetMesh())
	{
		return;
	}

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

	Weapon->SetActorRelativeLocation(Offset.GetLocation());

	Weapon->SetActorRelativeRotation(Offset.GetRotation());
}

void ALunarAsylumCharacter::UpdateWeaponTransform(float DeltaTime)
{
	if (!bIsInterpWeaponTransform || !CurrentWeapon || !CurrentWeapon->Mesh)
	{
		return;
	}

	FVector CurrentLoc = CurrentWeapon->GetRootComponent()->GetRelativeLocation();
	FRotator CurrentRot = CurrentWeapon->GetRootComponent()->GetRelativeRotation();

	FVector TargetLoc = TargetWeaponTransform.GetLocation();
	FRotator TargetRot = TargetWeaponTransform.GetRotation().Rotator();


	FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, WeaponInterpSpeed);
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, WeaponInterpSpeed);

	CurrentWeapon->GetRootComponent()->SetRelativeLocation(NewLoc);
	CurrentWeapon->GetRootComponent()->SetRelativeRotation(NewRot);

	if (NewLoc.Equals(TargetLoc, 0.1f) && NewRot.Equals(TargetRot, 0.1f))
	{
		bIsInterpWeaponTransform = false;
		CurrentWeapon->GetRootComponent()->SetRelativeLocation(TargetLoc);
		CurrentWeapon->GetRootComponent()->SetRelativeRotation(TargetRot);
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
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	// 화면 크기 가져오기
	int32 ScreenWidth, ScreenHeight;
	PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

	// 화면 정중앙 2D 좌표 계산
	FVector2D ScreenCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);

	FVector WorldLocation, WorldDirection;

	// 2D 화면 중앙을 3D 월드 시작점과 방향 벡터로 변환
	if (PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
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
		DrawDebugSphere(GetWorld(), HitResult.Location, 20.f, 12, bHit ? FColor::Green : FColor::Red, false, 0.1f);

		if (bHit)
		{
			AItemBase* InteractionItem = Cast<AItemBase>(HitResult.GetActor());
			if (InteractionItem && TargetItem != InteractionItem)
			{
				TargetItem = InteractionItem;
				OnTargetItemChanged.Broadcast(TargetItem->ItemName.ToString());
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

void ALunarAsylumCharacter::DropPrimaryWeapon()
{
	if (CurrentEquipState != EEquipState::Primary)
	{
		DropWeapon(EEquipState::Primary);
	}
}

void ALunarAsylumCharacter::DropSecondaryWeapon()
{
	if (CurrentEquipState != EEquipState::Secondary)
	{
		DropWeapon(EEquipState::Secondary);
	}
}

void ALunarAsylumCharacter::DropWeapon(EEquipState EquipState)
{
	ASandboxWeaponBase* WeaponToDrop = nullptr;

	if (EquipState == EEquipState::Primary)
	{
		WeaponToDrop = PrimaryWeapon;
		PrimaryWeapon = nullptr;
	}
	else if (EquipState == EEquipState::Secondary)
	{
		WeaponToDrop = SecondaryWeapon;
		SecondaryWeapon = nullptr;
	}

	if (!WeaponToDrop)
	{
		return;
	}

	WeaponToDrop->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	WeaponToDrop->SetActorEnableCollision(true);
	WeaponToDrop->SetOwner(nullptr);

	WeaponToDrop->Mesh->SetCollisionProfileName(TEXT("Weapon"));
	WeaponToDrop->Mesh->SetSimulatePhysics(true);

	FVector DropDirection = GetActorForwardVector() + GetActorUpVector() * 0.2f; // 앞 + 위쪽 대각선 방향
	DropDirection.Normalize();
	float LaunchForce = 500.0f; // 던지는 힘 세기 

	// 무기 메쉬에 순간적인 충격량을 가한다.
	WeaponToDrop->Mesh->AddImpulse(DropDirection * LaunchForce, NAME_None, true);

	OnItemAcquired.Broadcast(FString::Printf(TEXT("%s 버림"), *WeaponToDrop->ItemName.ToString()));
}

void ALunarAsylumCharacter::HitReaction()
{
	//if (CurrentActionState == EActionState::Dead)
	//{
	//	return;
	//}
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

void ALunarAsylumCharacter::OnInteractStarted()
{
	if (TargetItem)
	{
		InteractionProgressPercent = 0.f;
		OnInteractionProgressChanged.Broadcast(InteractionProgressPercent);
	}
}

void ALunarAsylumCharacter::OnInteractOngoing(const FInputActionInstance& Instance)
{
	if (!TargetItem)
	{
		return;
	}

	float ElapsedTime = Instance.GetElapsedTime();

	InteractionProgressPercent = FMath::Clamp(ElapsedTime / InteractHoldTimeDuration, 0.f, 1.f);
	OnInteractionProgressChanged.Broadcast(InteractionProgressPercent);
	UE_LOG(LogTemp, Log, TEXT("먹기 : %f"), InteractionProgressPercent);
}

void ALunarAsylumCharacter::OnInteractTriggered()
{
	if (TargetItem)
	{
		Interaction();
		InteractionProgressPercent = 0.f;
		OnInteractionProgressChanged.Broadcast(InteractionProgressPercent);
	}
}

void ALunarAsylumCharacter::OnInteractCanceled()
{
	InteractionProgressPercent = 0.f;
	OnInteractionProgressChanged.Broadcast(InteractionProgressPercent);
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
