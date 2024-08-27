// Copyright, Anon Creation Games

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestSystemStruct.h"
#include "Components/ActorComponent.h"
#include "QuestManager.generated.h"

class AAudioActor;
class UCutsceneWidget;

/** This should be attached to the GameMode */
UCLASS( ClassGroup=(Anon), meta=(BlueprintSpawnableComponent) )
class ANONQUESTSYSTEM_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuestManager();

	// ==================== Lifecycles ==================== //
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// ==================== References ==================== //

	/** Actor that accepts the quest */
	TWeakObjectPtr<APawn> AcceptingPawn;
	
	// ==================== Quests ==================== //

	UPROPERTY(EditDefaultsOnly, Category=Quests)
	TObjectPtr<UDataTable> QuestsData;
	
	FQuestInfo* ActiveQuest = nullptr;
	
	void FinishQuest();
	
public:

	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FName QuestNum, APawn* InPawn);

private:
	// ==================== Objectives ==================== //

	TArray<FQuestObjective> Objectives;

	/** Current Objective */
	int32 Curr = 0;

	void ActivateCurrentObjective();

	/** Continue or finish the quest */
	void ContinueObjective();

	void PrepareObjectives();

private:
	// ==================== Involved Actors ==================== //

	UPROPERTY()
	TMap<int16, TObjectPtr<AActor>> InvolvedActors;

	void SpawnInvolvedActors();

	/** Destroy if any */
	void DestroyInvolvedActors();
	
	// ==================== Marker ==================== //

	UPROPERTY(EditDefaultsOnly, Category=Marker)
	TSubclassOf<AActor> MarkerClass;

	UPROPERTY()
	TObjectPtr<AActor> Marker;

	void PrepareMarker();

	UFUNCTION()
	void MarkerOnOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
private:
	// ==================== Cutscene Handler ==================== //

	/** Using widgets to play a cutscene */
	UPROPERTY(EditDefaultsOnly, Category=Cutscene)
	TSubclassOf<UCutsceneWidget> CutsceneWidgetClass;

	UPROPERTY()
	TObjectPtr<UCutsceneWidget> CutsceneWidget;

	/** Using actor to play audio */
	UPROPERTY(EditDefaultsOnly, Category=Cutscene)
	TSubclassOf<AAudioActor> AudioActorClass;

	UPROPERTY()
	TObjectPtr<AAudioActor> AudioActor;

	FTimerHandle CutsceneFinishTimer;

	void PlayCutscene();
	void OnCutsceneFinished();
};
