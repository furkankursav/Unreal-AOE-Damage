// Fill out your copyright notice in the Description page of Project Settings.


#include "UAOEGameStateBase.h"

#include "Damageable.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void AUAOEGameStateBase::SpawnAOE(FVector AOESpawnLocation, float AOERadius, bool bRequireLOS, AActor* AOEInstigator,
                                  FOnAOESpawned OnAoeSpawnedDelegate, float DrawDebugDuration, FColor DebugColor)
{
	//TODO: bind delegate to function
	OnAoeSpawnedDelegate.AddDynamic(this, &AUAOEGameStateBase::AUAOEGameStateBase::FunctionToBind);

	if(DrawDebugDuration > 0.f)
	{
		DrawDebugSphere(GetWorld(), AOESpawnLocation, AOERadius, 16.f, DebugColor, false, DrawDebugDuration, 0.f, 2.f);
	}


	TArray<TEnumAsByte<EObjectTypeQuery>> AOEHitObjectTypes;
	AOEHitObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(AOEInstigator);

	TArray<AActor*> OutActors;


	UClass* AOEEffectClass = AActor::StaticClass();
	
	UKismetSystemLibrary::SphereOverlapActors(this, AOESpawnLocation, AOERadius, AOEHitObjectTypes, AOEEffectClass, IgnoreActors, OutActors);

	for(AActor* CurrentActor : OutActors)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(CurrentActor, UDamageable::StaticClass()))
		{

			if(bRequireLOS)
			{
				FHitResult LOSHitResult;
				GetWorld()->LineTraceSingleByChannel(LOSHitResult, AOESpawnLocation, CurrentActor->GetActorLocation(), ECC_Visibility);

				if(!LOSHitResult.bBlockingHit)
				{
					OnAoeSpawnedDelegate.Broadcast(CurrentActor);
				}
			}
			else
			{
				OnAoeSpawnedDelegate.Broadcast(CurrentActor);
			}
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Does not implement interface!"));
		}
	}

	

	//TODO: undind delegate from function
	OnAoeSpawnedDelegate.RemoveDynamic(this, &AUAOEGameStateBase::FunctionToBind);
}

void AUAOEGameStateBase::FunctionToBind(AActor* HitActor)
{
	// You can do some common things here!
}
