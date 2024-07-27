// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/USDFCharacterControlData.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Physics/USDFCollision.h"
#include "Kismet/GameplayStatics.h"
#include "UI/USDFPlayerHpBarWidget.h"
#include "Player/USDFPlayerController.h"
#include "Game/USDFGameMode.h"
#include "Interface/USDFGameModeInterface.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "CharacterStat/USDFPlayerStatComponent.h"
#include "Engine/OverlapResult.h"
#include "Damage/USDFDamageSystemComponent.h"
#include "Animation/USDFPlayerAnimInstance.h"

AUSDFCharacterPlayer::AUSDFCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Stat = CreateDefaultSubobject<UUSDFPlayerStatComponent>(TEXT("Stat"));

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 85.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_USDF_PlAYER_CAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName("NoCollision");

	static ConstructorHelpers::FObjectFinder<UUSDFCharacterControlData> PreviewDataAssetRef(TEXT("/Game/CharacterControl/CDA_Preview.CDA_Preview"));
	if (PreviewDataAssetRef.Object)
	{
		CharacterControlManager.Add(ECharacterPlayerControlType::Preview, PreviewDataAssetRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DodgeActionRef(TEXT("/Game/Input/Actions/IA_Dodge.IA_Dodge"));
	if (DodgeActionRef.Object)
	{
		DodgeAction = DodgeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackQKeyActionRef(TEXT("/Game/Input/Actions/IA_AttackQ.IA_AttackQ"));
	if (AttackQKeyActionRef.Object)
	{
		AttackQKeyAction = AttackQKeyActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackEKeyActionRef(TEXT("/Game/Input/Actions/IA_AttackE.IA_AttackE"));
	if (AttackEKeyActionRef.Object)
	{
		AttackEKeyAction = AttackEKeyActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackRKeyActionRef(TEXT("/Game/Input/Actions/IA_AttackR.IA_AttackR"));
	if (AttackRKeyActionRef.Object)
	{
		AttackRKeyAction = AttackRKeyActionRef.Object;
	}

	CurrentControlType = ECharacterPlayerControlType::Preview;
}

void AUSDFCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentControlType);

	DamageSystem->OnDeath.BindUObject(this, &AUSDFCharacterPlayer::OnDeath);
	DamageSystem->OnDamageResponse.BindUObject(this, &AUSDFCharacterPlayer::OnDamageResponse);

	bBoneLayeredBlendEnable = true;
}

void AUSDFCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AUSDFCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUSDFCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayer::ReleaseMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::Look);
}

void AUSDFCharacterPlayer::SetCharacterControl(ECharacterPlayerControlType NewCharacterControlType)
{	
	UUSDFCharacterControlData* CharacterControlData = CharacterControlManager[NewCharacterControlType];
	SetCharacterControlData(CharacterControlData);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		UInputMappingContext* NewInputMappingContext = CharacterControlData->InputMappingContext;
		SubSystem->AddMappingContext(NewInputMappingContext,0);
	}
}

void AUSDFCharacterPlayer::SetCharacterControlData(const UUSDFCharacterControlData* NewCharacterControlData)
{
	Super::SetCharacterControlData(NewCharacterControlData);

	GetCharacterMovement()->bOrientRotationToMovement = NewCharacterControlData->bOrientRotationToMovement;

	SpringArm->TargetArmLength = NewCharacterControlData->TargetArmLength;
	SpringArm->bUsePawnControlRotation = NewCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = NewCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = NewCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = NewCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = NewCharacterControlData->bDoCollisionTest;
	SpringArm->SetRelativeRotation(NewCharacterControlData->RelativeRotation);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->ViewPitchMax = NewCharacterControlData->ViewPitchMax;
		PlayerController->PlayerCameraManager->ViewPitchMin = NewCharacterControlData->ViewPitchMin;
	}
}

void AUSDFCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Type::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Type::Y);

	AddMovementInput(ForwardVector, MovementVector.X);
	AddMovementInput(RightVector, MovementVector.Y);

	MovementInputValue = MovementVector;
}

void AUSDFCharacterPlayer::ReleaseMove(const FInputActionValue& value)
{
	MovementInputValue = FVector2D::ZeroVector;
}

void AUSDFCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X * 45 * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(-LookAxisVector.Y * 45 *GetWorld()->GetDeltaSeconds());
}

void AUSDFCharacterPlayer::Dodge()
{
	bDodgeKeyPress = true;
}

void AUSDFCharacterPlayer::StopDodge()
{
	bDodgeKeyPress = false;
}

void AUSDFCharacterPlayer::Attack()
{
	bAttackKeyPress = true;
}

void AUSDFCharacterPlayer::ReleaseAttack()
{
	bAttackKeyPress = false;
}

void AUSDFCharacterPlayer::AttackQKey()
{
	bAttackQKeyPress = true;
}

void AUSDFCharacterPlayer::ReleaseAttackQKey()
{
	bAttackQKeyPress = false;
}

void AUSDFCharacterPlayer::AttackEKey()
{
	bAttackEKeyPress = true;
}

void AUSDFCharacterPlayer::ReleaseAttackEKey()
{
	bAttackEKeyPress = false;
}

void AUSDFCharacterPlayer::AttackRKey()
{
	bAttackRKeyPress = true;
}

void AUSDFCharacterPlayer::ReleaseAttackRKey()
{
	bAttackRKeyPress = false;
}

APlayerController* AUSDFCharacterPlayer::GetPlayerController()
{
	return Cast<APlayerController>(GetController());
}

FVector2D AUSDFCharacterPlayer::GetMovementInputValue()
{
	return MovementInputValue;
}

bool AUSDFCharacterPlayer::IsAttackState()
{
	return bAttackState;
}

bool AUSDFCharacterPlayer::IsDeadState()
{
	return Stat->GetCurrentHp() <= 0.0f;
}

void AUSDFCharacterPlayer::SetBoneLayeredBlendEnable(bool NewBoneLayeredBlendEnable)
{
	bBoneLayeredBlendEnable = NewBoneLayeredBlendEnable; 
	
	UUSDFPlayerAnimInstance* AnimInstance = Cast<UUSDFPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetBondLayeredBlendEnable(bBoneLayeredBlendEnable);
	}
}


void AUSDFCharacterPlayer::SetupPlayerHpBarHUDWidget(UUSDFPlayerHpBarWidget* HpBar)
{
	if (HpBar)
	{
		Stat->OnHpChangedDelegate.AddUObject(HpBar, &UUSDFPlayerHpBarWidget::UpdateHpBar);
		HpBar->SetMaxHp(Stat->GetMaxHp());
		HpBar->UpdateHpBar(Stat->GetCurrentHp());
	}
}

float AUSDFCharacterPlayer::GetCurrentHealth()
{
	return Stat->GetCurrentHp();
}

float AUSDFCharacterPlayer::GetMaxHealth()
{
	return Stat->GetMaxHp();
}

void AUSDFCharacterPlayer::Heal(float HealAmount)
{
	DamageSystem->Heal(Stat, HealAmount);
}

void AUSDFCharacterPlayer::TakeDamage(FDamageInfo DamageInfo)
{
	DamageSystem->TakeDamage(Stat, DamageInfo);
}


bool AUSDFCharacterPlayer::IsDead()
{
	return DamageSystem->GetIsDead();
}

void AUSDFCharacterPlayer::OnDeath()
{
	Super::OnDeath();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
		if (GameModeInterface)
		{
			GameModeInterface->OnPlayerDead();
		}
	}
}

void AUSDFCharacterPlayer::OnDamageResponse(FDamageInfo DamageInfo)
{
	Super::OnDamageResponse(DamageInfo);

	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(GetController());
	if (PlayerController == nullptr)
		return;

	FAISensedParam Param = {};
	Param.DamageInfo = DamageInfo;

	AActor* HitCauser = DamageInfo.DamageCauser;

	UUSDFPlayerAnimInstance* PlayerAnimInstance = Cast<UUSDFPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnimInstance == nullptr)
		return;

	if (HitCauser == nullptr)
		return;

	if (Stat->GetCurrentHp() <= 0.0f)
		return;

	PlayerAnimInstance->StopAllMontages(0.0f);

	switch (DamageInfo.DamageType)
	{
		case EDamageType::HitDefault:
		{
			FVector ActorForwardVector = GetActorForwardVector();
			FVector ActorRightVector = GetActorRightVector();
			FVector HitActorForwardVector = HitCauser->GetActorForwardVector();

			float ForwardDotProduct = ActorForwardVector.Dot(HitActorForwardVector);
			float RightDotProduct = ActorRightVector.Dot(HitActorForwardVector);

			UAnimMontage* HitReactMontage = nullptr;
			if (ForwardDotProduct >= 0.5f)
			{
				HitReactMontage = HitReactAnimMontage[EHitReactDirection::Front];
			}
			else if (ForwardDotProduct <= -0.5f)
			{
				HitReactMontage = HitReactAnimMontage[EHitReactDirection::Back];
			}
			else if (RightDotProduct >= 0.0f)
			{
				HitReactMontage = HitReactAnimMontage[EHitReactDirection::Right];
			}
			else
			{
				HitReactMontage = HitReactAnimMontage[EHitReactDirection::Left];
			}

			if (HitReactMontage)
			{
				bDamagedState = true;
				PlayerAnimInstance->Montage_Play(HitReactMontage);

				FOnMontageBlendingOutStarted BlendOutDelegate;
				BlendOutDelegate.BindUObject(this, &AUSDFCharacterPlayer::OnHitReactMontageBlendOut);

				PlayerAnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, HitReactMontage);
			}
		}
			break;
		case EDamageType::HitKnockback:
		{
			if (HitReactKnockBackMontage)
			{
				bDamagedState = true;
				PlayerAnimInstance->Montage_Play(HitReactKnockBackMontage);

				FOnMontageBlendingOutStarted BlendOutDelegate;
				BlendOutDelegate.BindUObject(this, &AUSDFCharacterPlayer::OnHitReactMontageBlendOut);

				PlayerAnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, HitReactKnockBackMontage);
			}
		}
			break;
		default:
			break;
	}
}

void AUSDFCharacterPlayer::OnHitReactMontageBlendOut(UAnimMontage* TargetMontage, bool bInterrupted)
{
	bDamagedState = false;
}
