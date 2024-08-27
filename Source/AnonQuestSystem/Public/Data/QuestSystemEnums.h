// Copyright, Anon Creation Games

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainQuest,
	SideQuest,
	Event
};

UENUM(BlueprintType)
enum class EObjectiveGoal : uint8
{
	Items, // Checking player inventory by tick interval, If required item is all met, objective is done
	Marker, // Simply go to marker
	Cutscene, // Just wait the cutscene to be finished
	Enemy // Checking how many enemy has the player beaten up.
};

UENUM(BlueprintType)
enum class EQuestArea : uint8
{
	None,	
};

