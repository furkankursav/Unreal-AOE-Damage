// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealAOEDamageCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HealthBarWidget.h"
#include "UAOEGameStateBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealAOEDamageCharacter

AUnrealAOEDamageCharacter::AUnrealAOEDamageCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	MyHealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MyHealthBarWidget"));
	MyHealthBarWidget->SetupAttachment(GetRootComponent());
	MyHealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	MyHealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MyHealthBarWidget->SetDrawAtDesiredSize(true);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthWidgetObj(TEXT("/Game/_Game/UI/WBP_Healthbar"));

	if(HealthWidgetObj.Succeeded())
	{
		MyHealthBarWidget->SetWidgetClass(HealthWidgetObj.Class);
	}

	
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealAOEDamageCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealAOEDamageCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealAOEDamageCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnrealAOEDamageCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnrealAOEDamageCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("DamageAction", IE_Pressed, this, &AUnrealAOEDamageCharacter::OnDamageActionPressed);
	PlayerInputComponent->BindAction("HealAction", IE_Pressed, this, &AUnrealAOEDamageCharacter::OnHealActionPressed);

	
}

void AUnrealAOEDamageCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if(MyHealthBarWidget != nullptr)
	{
		HealthBar = Cast<UHealthBarWidget>(MyHealthBarWidget->GetUserWidgetObject());

		if(HealthBar)
		{
			HealthBar->HealthInterface = this;
		}
	}

	OnAoeSpawned.AddDynamic(this, &AUnrealAOEDamageCharacter::OnDamageActionAOESpawned);
	OnHealAOESpawned.AddDynamic(this, &AUnrealAOEDamageCharacter::OnHealActionAOESpawned);
	
}


void AUnrealAOEDamageCharacter::Heal(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
}

void AUnrealAOEDamageCharacter::Damage(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.f, MaxHealth);
}

float AUnrealAOEDamageCharacter::GetHealthAsRatio()
{
	return CurrentHealth / MaxHealth;
}

void AUnrealAOEDamageCharacter::OnDamageActionAOESpawned(AActor* HitActor)
{

	IDamageable* ActorToDamage = Cast<IDamageable>(HitActor);

	if(ActorToDamage)
	{
		ActorToDamage->Damage(20.f);
	}
}

void AUnrealAOEDamageCharacter::OnDamageActionPressed()
{
	
	if(AUAOEGameStateBase* MyGameState = Cast<AUAOEGameStateBase>(UGameplayStatics::GetGameState(this)))
		MyGameState->SpawnAOE(GetActorLocation(), 400.f, true, this, OnAoeSpawned, 2.5f, FColor::Red);
}

void AUnrealAOEDamageCharacter::OnHealActionPressed()
{
	if(AUAOEGameStateBase* MyGameState = Cast<AUAOEGameStateBase>(UGameplayStatics::GetGameState(this)))
		MyGameState->SpawnAOE(GetActorLocation(), 400.f, false, nullptr, OnHealAOESpawned, 2.5f, FColor::Green);
}

void AUnrealAOEDamageCharacter::OnHealActionAOESpawned(AActor* HitActor)
{
	IDamageable* ActorToDamage = Cast<IDamageable>(HitActor);

	if(ActorToDamage)
	{
		ActorToDamage->Heal(20.f);
	}
}


void AUnrealAOEDamageCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealAOEDamageCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealAOEDamageCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUnrealAOEDamageCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
