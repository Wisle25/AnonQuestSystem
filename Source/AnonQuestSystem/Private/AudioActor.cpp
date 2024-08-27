// Copyright, Anon Creation Games

#include "AudioActor.h"

#include "MediaSoundComponent.h"

AAudioActor::AAudioActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MediaSound = CreateDefaultSubobject<UMediaSoundComponent>("Media Sound");
}

