// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/USDFCharacterControlData.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AUSDFCharacterPlayer::AUSDFCharacterPlayer()
{
	// CDO
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 85.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/IdaFaber/Meshes/Girl/SK_CALISTA_01.SK_CALISTA_01"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/Player/ABP_USDFPlayerAnimInstance.ABP_USDFPlayerAnimInstance_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFCharacterControlData> ShoulderDataAssetRef(TEXT("/Game/CharacterControl/CDA_Shoulder.CDA_Shoulder"));
	if(ShoulderDataAssetRef.Object)
	{
		CharacterControlManager.Add(ECharacterPlayerControlType::Shoulder, ShoulderDataAssetRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderMoveActionRef(TEXT("/Game/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (ShoulderMoveActionRef.Object)
	{
		ShoulderMoveAction = ShoulderMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookActionRef(TEXT("/Game/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (ShoulderLookActionRef.Object)
	{
		ShoulderLookAction = ShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	CurrentControlType = ECharacterPlayerControlType::Shoulder;
}

void AUSDFCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentControlType);
}

void AUSDFCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayer::StopJumping);
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
	//Pawn
	bUseControllerRotationYaw = NewCharacterControlData->bUseControllerRotationYaw;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = NewCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->RotationRate = NewCharacterControlData->RotationRate;
	GetCharacterMovement()->bUseControllerDesiredRotation = NewCharacterControlData->bUseControllerDesiredRotation;

	SpringArm->TargetArmLength = NewCharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(NewCharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = NewCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = NewCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = NewCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = NewCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = NewCharacterControlData->bDoCollisionTest;
}

void AUSDFCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.X);
	AddMovementInput(RightVector, MovementVector.Y);
}

void AUSDFCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}
