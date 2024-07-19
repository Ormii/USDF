// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/USDFAICommon.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "Interface/USDFCharacterPlayerInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"

AUSDFAIController::AUSDFAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	SightConfig->SightRadius = 800;
	SightConfig->LoseSightRadius = 1200;
	SightConfig->PeripheralVisionAngleDegrees = 60;
	SightConfig->SetMaxAge(5);

	HearingConfig->HearingRange = 500;
	HearingConfig->SetMaxAge(3.0f);

	DamageConfig->SetMaxAge(5.0f);

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->ConfigureSense(*HearingConfig);
	AIPerception->ConfigureSense(*DamageConfig);
	
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/BB_USDF_MeleeMonster.BB_USDF_MeleeMonster"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/BT_USDF_MeleeMonster.BT_USDF_MeleeMonster"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AUSDFAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUSDFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AUSDFAIController::OnPerceptionUpdated);

	RunAI();

}

bool AUSDFAIController::CanSenseActor(AActor* InActor, EAISense AISense, FAIStimulus& OutStimulus)
{
	FActorPerceptionBlueprintInfo Info = {};
	AIPerception->GetActorsPerception(InActor, Info);

	FAISenseID AISenseID = FAISenseID::FAINamedID();
	FAIStimulus::FResult result = FAIStimulus::FResult::SensingFailed;
	switch (AISense)
	{
		case EAISense::Sight:
			AISenseID = SightConfig->GetSenseID();
			break;
		case EAISense::Hearing:
			AISenseID = HearingConfig->GetSenseID();
			break;
		case EAISense::Damage:
			AISenseID = DamageConfig->GetSenseID();
			break;
		default:
			break;
	}

	for (int32 i = 0; i < Info.LastSensedStimuli.Num(); ++i)
	{
		FAIStimulus Stimulus = Info.LastSensedStimuli[i];
		if (Stimulus.Type == AISenseID)
		{
			OutStimulus = Stimulus;
			return Stimulus.IsActive();
		}
	}

	return false;

}

void AUSDFAIController::HandleSensedSight(AActor* InActor)
{
	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(GetPawn());
	if (AIPawn == nullptr)
		return;

	IUSDFCharacterPlayerInterface* Player = Cast<IUSDFCharacterPlayerInterface>(InActor);
	if (Player == nullptr)
		return;

	FAISensedParam Param = {};
	Param.Actor = InActor;

	AttackTarget = InActor;

	EAIState AIState = GetCurrentAIState();
	switch (AIState)
	{
		case EAIState::Passive:
		case EAIState::Investigating:
			StopMovement();
			SetCurrentAIState(EAIState::Attacking, Param);
			AIPawn->SetAIState(EAIState::Attacking, Param);
			break;
		default:
			break;
	}
}

void AUSDFAIController::HandleSensedSound(FVector Location)
{
	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(GetPawn());
	if (AIPawn == nullptr)
		return;

	FAISensedParam Param = {};
	Param.Location = Location;

	EAIState AIState = GetCurrentAIState();
	switch (AIState)
	{
		case EAIState::Passive:
		case EAIState::Investigating:
			StopMovement();
			SetCurrentAIState(EAIState::Investigating, Param);
			Blackboard->SetValueAsVector(BBKEY_POINT_OF_INTREST, Location);
			AIPawn->SetAIState(EAIState::Investigating, Param);
			break;
		default:
			break;
	}
}

void AUSDFAIController::HandleSensedDamage(AActor* InActor)
{
	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(GetPawn());
	if (AIPawn == nullptr)
		return;

	IUSDFCharacterPlayerInterface* Player = Cast<IUSDFCharacterPlayerInterface>(InActor);
	if (Player == nullptr)
		return;

	FAISensedParam Param = {};
	Param.Actor = InActor;
	EAIState AIState = GetCurrentAIState();
	switch (AIState)
	{
	case EAIState::Passive:
	case EAIState::Investigating:
		StopMovement();
		SetCurrentAIState(EAIState::Attacking,Param);
		AIPawn->SetAIState(EAIState::Attacking, Param);
		break;
	default:
		break;
	}
}

void AUSDFAIController::SetCurrentAIState(EAIState NewState, FAISensedParam Param)
{
	Blackboard->SetValueAsEnum(BBKEY_AISTATE, static_cast<uint8>(NewState));
	switch (NewState)
	{
		case EAIState::Attacking:
		{
			if(Param.bUseLastAttackTarget == false)
				Blackboard->SetValueAsObject(BBKEY_ATTACK_TARGET, Param.Actor);
		}
			break;
		default:
			break;
	}
}

EAIState AUSDFAIController::GetCurrentAIState()
{
	EAIState CurrentAIState = static_cast<EAIState>(Blackboard->GetValueAsEnum(BBKEY_AISTATE));
	return CurrentAIState;
}

const AActor* AUSDFAIController::GetAttackTarget() const
{
	return dynamic_cast<AActor*>(Blackboard->GetValueAsObject(BBKEY_ATTACK_TARGET));
}

void AUSDFAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AUSDFAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree(EBTStopMode::Safe);
	}
}

void AUSDFAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (int32 i = 0; i < UpdatedActors.Num(); ++i)
	{
		{
			FAIStimulus Stimulus = {};
			if (CanSenseActor(UpdatedActors[i], EAISense::Sight, Stimulus))
			{
				HandleSensedSight(UpdatedActors[i]);
			}
		}

		{
			FAIStimulus Stimulus = {};
			if (CanSenseActor(UpdatedActors[i], EAISense::Hearing, Stimulus))
			{
				HandleSensedSound(Stimulus.StimulusLocation);
			}
		}

		{
			FAIStimulus Stimulus = {};
			if (CanSenseActor(UpdatedActors[i], EAISense::Damage, Stimulus))
			{
				HandleSensedDamage(UpdatedActors[i]);
			}
		}
	}
}
