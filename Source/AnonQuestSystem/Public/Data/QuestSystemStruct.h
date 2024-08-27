// Copyright, Anon Creation Games

#pragma once

#include "CoreMinimal.h"
#include "MediaPlayer.h"
#include "QuestSystemEnums.h"
#include "QuestSystemStruct.generated.h"

USTRUCT(BlueprintType)
struct FQuestRewards
{
	GENERATED_BODY()

	// Like an XP, Gold, or Whatever

	UPROPERTY(EditDefaultsOnly)
	int32 XP = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 Gold = 0;
};

USTRUCT(BlueprintType)
struct FInvolvedActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditDefaultsOnly)
	FTransform ActorTransform;
};

USTRUCT(BlueprintType)
struct FCutsceneData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> MaterialCutscene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMediaPlayer> MediaPlayerCutscene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMediaSource> MediaSourceCutscene;

	UPROPERTY(EditDefaultsOnly)
	float Duration = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	bool bSkipable = true;
};

USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category=General)
	FName ObjectiveName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category=General)
	FString Description = TEXT("");

	UPROPERTY(EditDefaultsOnly, Category=General)
	TArray<FString> Tips;

	UPROPERTY(EditDefaultsOnly, Category=Goals)
	EObjectiveGoal ObjectiveGoal = EObjectiveGoal::Items;

	// ===== Events (What happened when objective is active) ===== //

	UPROPERTY(EditDefaultsOnly, Category=Events, meta=(EditCondition="ObjectiveGoal!=EObjectiveGoal::Cutscene", EditConditionHides))
	bool bReplacePlayerToCertainPoint = false;

	UPROPERTY(EditDefaultsOnly, Category=Events, meta=(EditCondition="bReplacePlayerToCertainPoint", EditConditionHides))
	FVector PlayerReplacementPoint = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category=Events, meta=(EditCondition="bReplacePlayerToCertainPoint", EditConditionHides))
	FRotator PlayerReplacementRotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditDefaultsOnly, Category=Events, meta=(EditCondition="ObjectiveGoal!=EObjectiveGoal::Cutscene", EditConditionHides))
	TArray<FInvolvedActor> InvolvedActors;
	
	// ===== Items ===== //

	/** Current items can be checked via player's inventory */
	TMap<FName, int32> CurrentItems;
	
	UPROPERTY(EditDefaultsOnly, Category=Goals, meta=(EditCondition="ObjectiveGoal==EObjectiveGoal::Items", EditConditionHides))
	TMap<FName, int32> RequiredItems;
	
	// ===== Marker ===== //
	
	/** If the objective goal is marker, then this SHOULD be true */
	UPROPERTY(EditDefaultsOnly, Category=Marker, meta=(EditCondition="ObjectiveGoal!=EObjectiveGoal::Cutscene", EditConditionHides))
	bool bHasMarker = false;

	UPROPERTY(EditDefaultsOnly, Category=Marker, meta=(EditCondition="bHasMarker", EditConditionHides))
	FVector MarkerLocation = FVector::ZeroVector;

	// ===== Cutscene ===== //
	
	UPROPERTY(EditDefaultsOnly, Category=Goals, meta=(EditCondition="ObjectiveGoal==EObjectiveGoal::Cutscene", EditConditionHides))
	FCutsceneData CutsceneData;
};

USTRUCT(BlueprintType)
struct FQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName QuestName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	EQuestType QuestType = EQuestType::MainQuest;

	UPROPERTY(EditDefaultsOnly)
	FString Description = TEXT("");

	UPROPERTY(EditDefaultsOnly)
	bool bRepeatable = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<FQuestObjective> Objectives;
};
