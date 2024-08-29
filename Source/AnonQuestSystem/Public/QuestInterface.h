// Copyright, Anon Creation Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UQuestInterface : public UInterface
{
	GENERATED_BODY()
};

class ANONQUESTSYSTEM_API IQuestInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetItemCountAtInventory(const FName& ItemName) = 0; 
};
