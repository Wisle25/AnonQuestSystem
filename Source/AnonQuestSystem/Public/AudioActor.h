// Copyright, Anon Creation Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioActor.generated.h"

class UMediaSoundComponent;

UCLASS()
class ANONQUESTSYSTEM_API AAudioActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioActor();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMediaSoundComponent> MediaSound;
};
