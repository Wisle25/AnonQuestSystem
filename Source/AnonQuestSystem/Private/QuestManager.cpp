// Copyright, Anon Creation Games

#include "QuestManager.h"

#include "AudioActor.h"
#include "CutsceneWidget.h"
#include "QuestInterface.h"
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

	CheckPawnInventory();
}

// ==================== Quest ==================== //

void UQuestManager::FinishQuest()
{
	ActiveQuest = nullptr;
	AcceptingPawn = nullptr;
}

void UQuestManager::AcceptQuest(FName QuestNum, APawn* InPawn)
{
	AcceptingPawn = InPawn;
	ActiveQuest = QuestsData->FindRow<FQuestInfo>(QuestNum, "");
	Objectives = ActiveQuest->Objectives;
	Curr = 0;

	ActivateCurrentObjective();
}

// ==================== Objectives ==================== //

void UQuestManager::ActivateCurrentObjective()
{
	if (!ActiveQuest) return;

	// Cutscene is handled different
	if (Objectives[Curr].ObjectiveGoal == EObjectiveGoal::Cutscene) PlayCutscene();
	else
	{
		PrepareObjectives();
		SetObjectiveGoal();
	}
}

void UQuestManager::ContinueObjective()
{
	DestroyInvolvedActors();
	
	++Curr;

	if (Curr < ActiveQuest->Objectives.Num()) ActivateCurrentObjective();
	else FinishQuest();
}

void UQuestManager::PrepareObjectives()
{
	if (Objectives[Curr].bReplacePlayerToCertainPoint)
	{
		AcceptingPawn->SetActorLocationAndRotation(Objectives[Curr].PlayerReplacementPoint, Objectives[Curr].PlayerReplacementRotation);
	}

	if (Objectives[Curr].bHasMarker) PrepareMarker();

	if (!Objectives[Curr].InvolvedActors.IsEmpty()) SpawnInvolvedActors();
}

void UQuestManager::SetObjectiveGoal()
{
	if (Objectives[Curr].ObjectiveGoal == EObjectiveGoal::Items)
	{
		SetComponentTickEnabled(true);

		QuestInterface = AcceptingPawn.Get();
		
		for (const auto& RequiredItem : Objectives[Curr].RequiredItems)
		{
			Objectives[Curr].CurrentItems.Add(RequiredItem.Key, 0);
		}
	}
}

// ==================== Enemy Handler ==================== //

void UQuestManager::UpdateEnemySlayed()
{
	if (Objectives[Curr].ObjectiveGoal != EObjectiveGoal::Enemy) return;

	++CurrentEnemySlayed;

	if (CurrentEnemySlayed >= Objectives[Curr].EnemyCount)
	{
		CurrentEnemySlayed = 0;
		ContinueObjective();
	}
}

// ==================== Item Handlers ==================== //

void UQuestManager::CheckPawnInventory()
{
	bool bHasFinished = true;
	
	for (const auto& RequiredItem : Objectives[Curr].RequiredItems)
	{
		int32 CurrentAmount = QuestInterface->GetItemCountAtInventory(RequiredItem.Key);
		Objectives[Curr].CurrentItems[RequiredItem.Key] = CurrentAmount;

		// Check the amount
		if (CurrentAmount < RequiredItem.Value)
		{
			bHasFinished = false;

			break;
		}
	}

	// Already obtains required amount
	if (bHasFinished)
	{
		SetComponentTickEnabled(false);
		QuestInterface = nullptr;
		ContinueObjective();
	}
}

// ==================== Involved Actors ==================== //

void UQuestManager::SpawnInvolvedActors()
{
	TArray<FInvolvedActor>& Actors = Objectives[Curr].InvolvedActors;
	
	for (int32 I = 0; I < Actors.Num(); ++I)
	{
		AActor* Involved = GetWorld()->SpawnActor<AActor>(Actors[I].ActorClass, Actors[I].ActorTransform);

		InvolvedActors.Add(I, Involved);
	}
}

void UQuestManager::DestroyInvolvedActors()
{
	TArray<FInvolvedActor>& Actors = Objectives[Curr].InvolvedActors;
	
	for (const auto& Actor : InvolvedActors)
	{
		// Destroy involved actors if is not needed anymore
		// Check is the actor still valid coz maybe its already destroyed either by killed or anything
		if (Actor.Value && Actor.Value->GetLifeSpan() == 0.f && Actors[Actor.Key].bDestroyAfterFinished)
		{
			Actor.Value->Destroy();
		}
	}

	InvolvedActors.Empty();
}

// ==================== Marker ==================== //

void UQuestManager::PrepareMarker()
{
	Marker = GetWorld()->SpawnActorDeferred<AActor>(MarkerClass, Objectives[Curr].MarkerTransform);
	Marker->OnActorBeginOverlap.AddDynamic(this, &ThisClass::MarkerOnOverlap);
	Marker->FinishSpawning(Objectives[Curr].MarkerTransform);
}

void UQuestManager::MarkerOnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
 	if (OtherActor != AcceptingPawn) return;
	
	Marker->Destroy();
	Marker = nullptr;
	
	if (Objectives[Curr].ObjectiveGoal == EObjectiveGoal::Marker)
	{
		ContinueObjective();
	}
}

// ==================== Cutscene Handler ==================== //

void UQuestManager::PlayCutscene()
{
	// Ignore any collision for the accepting pawn
	AcceptingPawn->SetActorEnableCollision(false);
	
	// The audio
	AudioActor = GetWorld()->SpawnActor<AAudioActor>(AudioActorClass);
	
	FCutsceneData CutsceneData = Objectives[Curr].CutsceneData;
	
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
	// Re enable collision
	AcceptingPawn->SetActorEnableCollision(true);
	
	// Remove cutscene
	CutsceneWidget->RemoveFromParent();
	CutsceneWidget = nullptr;

	AudioActor->Destroy();
	AudioActor = nullptr;

	// Continue Objective
	ContinueObjective();
}
