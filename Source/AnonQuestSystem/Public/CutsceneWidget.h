// Copyright, Anon Creation Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/QuestSystemStruct.h"
#include "CutsceneWidget.generated.h"

class AAudioActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutsceneFinishedSignature);

UCLASS()
class ANONQUESTSYSTEM_API UCutsceneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCutsceneFinishedSignature OnCutsceneFinished;
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayCutscene(const FCutsceneData& Cutscene, AAudioActor* AudioActor);
};
