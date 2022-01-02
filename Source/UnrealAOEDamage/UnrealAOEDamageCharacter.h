// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "UAOEGameStateBase.h"
#include "GameFramework/Character.h"
#include "UnrealAOEDamageCharacter.generated.h"

UCLASS(config=Game)
class AUnrealAOEDamageCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AUnrealAOEDamageCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/*
	 * Damageable Interface Functions
	 */
public:
	virtual void Heal(float Amount) override;
	virtual void Damage(float Amount) override;
	virtual float GetHealthAsRatio() override;

	/*
	 * AOE Damage Variables
	 */
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* MyHealthBarWidget;
	
	// Internal reference to the blueprint for gameplay logic
	UPROPERTY(VisibleAnywhere ,BlueprintReadWrite, Category = "UI")
	class UHealthBarWidget* HealthBar;


	FOnAOESpawned OnAoeSpawned;

	UFUNCTION()
	void OnDamageActionAOESpawned(AActor* HitActor);
	
	UFUNCTION()
	void OnDamageActionPressed();

	UFUNCTION()
	void OnHealActionPressed();


	FOnAOESpawned OnHealAOESpawned;
	
	UFUNCTION()
	void OnHealActionAOESpawned(AActor* HitActor);
};

