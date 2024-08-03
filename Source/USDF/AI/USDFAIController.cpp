// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/USDFAICommon.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "Interface/USDFCharacterPlayerInterface.h"
#include "Interface/USDFDamageableInterface.h"
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
	
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/MeleeMonster/Default/BB_USDFMeleeDefault.BB_USDFMeleeDefault"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/MeleeMonster/Default/BT_USDFMeleeDefault.BT_USDFMeleeDefault"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AUSDFAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EAIState AIState = static_cast<EAIState>(Blackboard->GetValueAsEnum(BBKEY_AISTATE));
	switch (AIState)
	{
		case EAIState::Attacking:
		{
			if (AttackTarget != nullptr)
			{
				IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(AttackTarget);
				if (Damageable && Damageable->IsDead())
				{
					AttackTarget = nullptr;
					SetCurrentAIState(EAIState::Passive, FAISensedParam{});
				}
			}
		}
			break;
		default:
			break;
	}
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
		case EAIState::Intresting:
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
		case EAIState::Intresting:
			SetCurrentAIState(EAIState::Intresting, Param);
			Blackboard->SetValueAsVector(BBKEY_POINT_OF_INTREST, Location);
			AIPawn->SetAIState(EAIState::Intresting, Param);
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
		case EAIState::Intresting:
			SetCurrentAIState(EAIState::Frozen,Param);
			AIPawn->SetAIState(EAIState::Frozen, Param);
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
			if(Param.bUseLastAttackTarget == false)
				Blackboard->SetValueAsObject(BBKEY_ATTACK_TARGET, Param.Actor);
			break;
		case EAIState::Frozen:
			Blackboard->SetValueAsObject(BBKEY_ATTACK_TARGET, Param.DamageInfo.DamageCauser);
			break;
		default:
			break;
	}
}

void AUSDFAIController::SetCurrentPhase(EGameStagePhase NewGameStagePhase)
{
	Blackboard->SetValueAsEnum(BBKEY_GAME_STAGE_PHASE, static_cast<uint8>(NewGameStagePhase));
}

void AUSDFAIController::SetTeleportState(bool NewTeleportState)
{
	Blackboard->SetValueAsBool(BBKEY_TELEPORT_STATE, NewTeleportState);
}

EAIState AUSDFAIController::GetCurrentAIState()
{
	EAIState CurrentAIState = static_cast<EAIState>(Blackboard->GetValueAsEnum(BBKEY_AISTATE));
	return CurrentAIState;
}

EGameStagePhase AUSDFAIController::GetCurrentPhase()
{
	EGameStagePhase CurrentGameStagePhase = static_cast<EGameStagePhase>(Blackboard->GetValueAsEnum(BBKEY_GAME_STAGE_PHASE));
	return CurrentGameStagePhase;
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
