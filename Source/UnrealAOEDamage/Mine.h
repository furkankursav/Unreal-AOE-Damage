// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAOEGameStateBase.h"
#include "GameFramework/Actor.h"
#include "Mine.generated.h"

UCLASS()
class UNREALAOEDAMAGE_API AMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mine")
	class UBoxComponent* BoxCollision;

	UPROPERTY()
	class AUAOEGameStateBase* MyGameState;

	UPROPERTY()
	FOnAOESpawned OnAoeSpawned;

	UFUNCTION()
	void OnExplosionAOESpawned(AActor* HitActor);

	UFUNCTION()
	void OnMineTriggered( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	bool bIsExploded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float MineRadius = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float MineDamage = 20.f;

};
