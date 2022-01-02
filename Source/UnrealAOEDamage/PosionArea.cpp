// Fill out your copyright notice in the Description page of Project Settings.


#include "PosionArea.h"

#include "Damageable.h"
#include "UAOEGameStateBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APosionArea::APosionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APosionArea::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = Cast<AUAOEGameStateBase>(UGameplayStatics::GetGameState(this));

	OnAoeSpawned.AddDynamic(this, &APosionArea::OnPosionAOESpawned);
	
}

void APosionArea::OnPosionAOESpawned(AActor* HitActor)
{
	IDamageable* ActorToDamage = Cast<IDamageable>(HitActor);

	if(ActorToDamage)
	{
		ActorToDamage->Damage(20.f * GetWorld()->DeltaTimeSeconds);
	}
}

// Called every frame
void APosionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MyGameState)
	{
		MyGameState->SpawnAOE(GetActorLocation(), MeshComp->Bounds.SphereRadius, false, this, OnAoeSpawned, 0.1f, FColor::Cyan);
	}

}

