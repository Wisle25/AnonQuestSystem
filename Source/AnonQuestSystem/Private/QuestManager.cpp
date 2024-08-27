// Copyright, Anon Creation Games

#include "QuestManager.h"

#include "AudioActor.h"
#include "CutsceneWidget.h"
#include "Blueprint/UserWidget.h"

UQuestManager::UQuestManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = .4f;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// ==================== Lifecycles ==================== //

void UQuestManager::BeginPlay()
{
	Super::BeginPlay();

}

void UQuestManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

// ==================== Quest ==================== //

void UQuestManager::ActivateCurrentObjective()
{
	if (!ActiveQuest) return;

	// Check which type is the objective
	if (ActiveQuest->Objectives[CurrentObjective].ObjectiveGoal == EObjectiveGoal::Cutscene)
	{
		PlayCutscene();
	}
}

void UQuestManager::ContinueObjective()
{
	++CurrentObjective;

	if (CurrentObjective < ActiveQuest->Objectives.Num()) ActivateCurrentObjective();
	else FinishQuest();
	
}

void UQuestManager::FinishQuest()
{
	ActiveQuest = nullptr;
	CurrentObjective = 0;
}

void UQuestManager::AcceptQuest(FName QuestNum)
{
	ActiveQuest = QuestsData->FindRow<FQuestInfo>(QuestNum, "");
	CurrentObjective = 0;

	ActivateCurrentObjective();
}

// ==================== Cutscene Handler ==================== //

void UQuestManager::PlayCutscene()
{
	// The audio
	AudioActor = GetWorld()->SpawnActor<AAudioActor>(AudioActorClass);

	FCutsceneData CutsceneData = ActiveQuest->Objectives[CurrentObjective].CutsceneData;
	
	// The Widget
	CutsceneWidget = CreateWidget<UCutsceneWidget>(GetWorld(), CutsceneWidgetClass);
	CutsceneWidget->PlayCutscene(CutsceneData, AudioActor);
	CutsceneWidget->AddToViewport();

	// Manual bind by using timer when the media is over
	const float Delay = CutsceneData.Duration;
	GetWorld()->GetTimerManager().SetTimer(CutsceneFinishTimer, this, &ThisClass::OnCutsceneFinished, Delay, false);
}

void UQuestManager::OnCutsceneFinished()
{
	// Remove cutscene
	CutsceneWidget->RemoveFromParent();
	CutsceneWidget = nullptr;

	AudioActor->Destroy();
	AudioActor = nullptr;

	// Continue Objective
	ContinueObjective();
}
