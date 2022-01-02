// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAOEGameStateBase.h"
#include "GameFramework/Actor.h"
#include "PosionArea.generated.h"

UCLASS()
class UNREALAOEDAMAGE_API APosionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APosionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY()
	class AUAOEGameStateBase* MyGameState;

	UPROPERTY()
	FOnAOESpawned OnAoeSpawned;

	UFUNCTION()
	void OnPosionAOESpawned(AActor* HitActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
