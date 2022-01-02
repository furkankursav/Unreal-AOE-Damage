// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UAOEGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAOESpawned, class AActor*, HitActor);

/**
 * 
 */
UCLASS()
class UNREALAOEDAMAGE_API AUAOEGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	
	
	UFUNCTION()
	void SpawnAOE(FVector AOESpawnLocation, float AOERadius, bool bRequireLOS, AActor* AOEInstigator, FOnAOESpawned OnAoeSpawnedDelegate, float DrawDebugDuration, FColor DebugColor);

	UFUNCTION()
	void FunctionToBind(AActor* HitActor);
	
};


