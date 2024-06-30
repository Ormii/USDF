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
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 85.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 340.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UUSDFCharacterControlData> ShoulderDataAssetRef(TEXT("/Game/CharacterControl/CDA_Shoulder.CDA_Shoulder"));
	if(ShoulderDataAssetRef.Object)
	{
		CharacterControlManager.Add(ECharacterPlayerControlType::Shoulder, ShoulderDataAssetRef.Object);
	}

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

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionRef(TEXT("/Game/Input/Actions/IA_Sprint.IA_Sprint"));
	if (SprintActionRef.Object)
	{
		SprintAction = SprintActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ViewChangeActionRef(TEXT("/Game/Input/Actions/IA_ViewChange.IA_ViewChange"));
	if (ViewChangeActionRef.Object)
	{
		ViewChangeAction = ViewChangeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	CurrentControlType = ECharacterPlayerControlType::Shoulder;
}

void AUSDFCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentControlType);
}

void AUSDFCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bSprintKeyPress)
		Acceleration = FMath::Clamp(Acceleration + AccelerationRate * DeltaSeconds, 0, MaxAcceleration);
	else
		Acceleration = 0;
}

void AUSDFCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayer::ReleaseMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayer::StopJumping);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayer::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayer::StopSprint);
	EnhancedInputComponent->BindAction(ViewChangeAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayer::PressViewChange);
	EnhancedInputComponent->BindAction(ViewChangeAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayer::ReleaseViweChange);
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

	SpringArm->TargetArmLength = NewCharacterControlData->TargetArmLength;
	SpringArm->bUsePawnControlRotation = NewCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = NewCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = NewCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = NewCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = NewCharacterControlData->bDoCollisionTest;
	SpringArm->SetRelativeRotation(NewCharacterControlData->RelativeRotation);
	MaxAcceleration = NewCharacterControlData->MaxAcceleration;
	AccelerationRate = NewCharacterControlData->AccelerationRate;
}

void AUSDFCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddVelocityScale = MovementVector;
	PreVelocity = GetCharacterMovement()->Velocity;
	PreGroundSpeed = GetCharacterMovement()->Velocity.Size2D();

	AddMovementInput(ForwardVector, MovementVector.X * (1 + Acceleration));
	AddMovementInput(RightVector, MovementVector.Y * (1 + Acceleration));
}

void AUSDFCharacterPlayer::ReleaseMove(const FInputActionValue& value)
{
	AddVelocityScale = FVector2D::ZeroVector;
	PreVelocity = FVector::ZeroVector;
	PreGroundSpeed = 0.0f;
}

void AUSDFCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}

void AUSDFCharacterPlayer::PressViewChange()
{
	SetCharacterControl(ECharacterPlayerControlType::Preview);
}

void AUSDFCharacterPlayer::ReleaseViweChange()
{
	SetCharacterControl(ECharacterPlayerControlType::Shoulder);
}

void AUSDFCharacterPlayer::Sprint()
{
	bSprintKeyPress = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AUSDFCharacterPlayer::StopSprint()
{
	bSprintKeyPress = false;
	GetCharacterMovement()->MaxWalkSpeed = 170.0f;
}

void AUSDFCharacterPlayer::Attack()
{

}

bool AUSDFCharacterPlayer::IsSprintState()
{
	return bSprintKeyPress;
}

bool AUSDFCharacterPlayer::IsCombatState()
{
	return bCombatState;
}

float AUSDFCharacterPlayer::GetPreGroundSpeed()
{
	return PreGroundSpeed;
}

FVector AUSDFCharacterPlayer::GetPreVelocity()
{
	return PreVelocity;
}

FVector2D AUSDFCharacterPlayer::GetAddVelocityScale()
{
	return AddVelocityScale;
}

void AUSDFCharacterPlayer::SetCombatState(bool NewCombatState)
{
	bCombatState = NewCombatState;
}
