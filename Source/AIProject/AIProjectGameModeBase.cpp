// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProjectGameModeBase.h"
#include "Engine.h"
#include "AIProject.h"

void AAIProjectGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		// Display a debug message for five seconds. 
		// The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello, this is AIProjectGameMode!"));
	}
}

void AAIProjectGameModeBase::SetDummyFreeState(bool flag) {
	bIsDummyFreeNow = flag;
}

bool AAIProjectGameModeBase::GetDummyFreeState() {
	return bIsDummyFreeNow;
}