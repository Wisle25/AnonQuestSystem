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
	// ==================== Quests ==================== //

	UPROPERTY(EditDefaultsOnly, Category=Quests)
	TObjectPtr<UDataTable> QuestsData;
	
	FQuestInfo* ActiveQuest = nullptr;

	int32 CurrentObjective = 0;

	void ActivateCurrentObjective();

	/** Continue or finish the quest */
	void ContinueObjective();

	void FinishQuest();
	
public:

	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FName QuestNum);
	
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
