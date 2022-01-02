// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine.h"
#include "UAOEGameStateBase.h"
#include "Components/BoxComponent.h"
#include "Damageable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AMine::AMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxCollision->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMine::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = Cast<AUAOEGameStateBase>(UGameplayStatics::GetGameState(this));

	OnAoeSpawned.AddDynamic(this, &AMine::OnExplosionAOESpawned);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMine::OnMineTriggered);
	
}

void AMine::OnExplosionAOESpawned(AActor* HitActor)
{
	IDamageable* ActorToDamage = Cast<IDamageable>(HitActor);

	if(ActorToDamage)
	{
		ActorToDamage->Damage(MineDamage);
	}
	
}

void AMine::OnMineTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(MyGameState && !bIsExploded)
	{
		MyGameState->SpawnAOE(GetActorLocation(), MineRadius, true, this, OnAoeSpawned, 3.f, FColor::Orange);
		bIsExploded = true;
		UMaterialInstanceDynamic* MineMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MeshComp->GetMaterial(0));
		MineMaterial->SetVectorParameterValue("Color", FVector(0.f, 1.f, 0.f));
		MeshComp->SetMaterial(0, MineMaterial);
	}
}

