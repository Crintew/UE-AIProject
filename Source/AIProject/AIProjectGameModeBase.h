// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API AAIProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;

public:

	bool bIsDummyFreeNow;

	UFUNCTION(BlueprintCallable)
	void SetDummyFreeState(bool flag);

	UFUNCTION(BlueprintCallable)
	bool GetDummyFreeState();
	
};
