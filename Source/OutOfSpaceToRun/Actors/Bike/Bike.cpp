// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/Actors/Bike/Bike.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ABike::ABike()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Defaults
	Fuel = 100.0f;
	FuelBurnRate = 1.0f;
	IsAlive = true;
	IsBoosting = false;
	DefaultMovementSpeed = 50.0f;
	MovementSpeedModifier = 1.0f;
	PanSpeed = 5.0f;
	
	MainChassis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Chassis"));
	MainChassis->SetupAttachment(GetCapsuleComponent());
	Engine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Engine"));
	Engine->SetupAttachment(MainChassis);
	HandleBars = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleBars"));
	HandleBars->SetupAttachment(MainChassis);
	FeetSupport = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FeetSupport"));
	FeetSupport->SetupAttachment(MainChassis);
	WheelsGuards = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelsGuards"));
	WheelsGuards->SetupAttachment(MainChassis);
	Wheels = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheels"));
	Wheels->SetupAttachment(MainChassis);
	Roof = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roof"));
	Roof->SetupAttachment(MainChassis);

	//// Setup parameters in editor
	//SpringArmMinimap = CreateDefaultSubobject<USpringArmComponent>(TEXT("Minimap Spring Arm"));
	//SpringArmMinimap->SetupAttachment(MainChassis);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Player Camera"));
	CameraBoom->SetupAttachment(MainChassis);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
}

// Called when the game starts or when spawned
void ABike::BeginPlay()
{
	Super::BeginPlay();
	


}

// Turns vehicle by 90 degrees
void ABike::Turn(const FInputActionValue& Value)
{
	if (!IsAlive)
		return;
	
	TObjectPtr<UPawnMovementComponent> MovementComponent = GetMovementComponent();
	if (!MovementComponent)
		return;

	if (!MovementComponent->IsMovingOnGround())
		return;
	float Val = Value.Get<float>();
	auto MainChassisRotation = MainChassis->GetComponentRotation();
	float OrigYaw = MainChassisRotation.Yaw;
	float NewYaw = OrigYaw + 90.0f * Val;
	NewYaw = FMath::Modulo(NewYaw, 360.0f);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,FString::Printf(TEXT("Turn: %f"), NewYaw));

	FRotator NewRotation( MainChassisRotation.Pitch, NewYaw, MainChassisRotation.Roll);
	MainChassis->SetWorldRotation(NewRotation);
	
}

void ABike::Pivot(const FInputActionValue& Value)
{
	if (!IsAlive)
		return;

	TObjectPtr<UPawnMovementComponent> MovementComponent = GetMovementComponent();
	if (!MovementComponent)
		return;

	if (!MovementComponent->IsMovingOnGround())
		return;

	float Val = Value.Get<float>();

	auto MainChassisRelativeRotation = MainChassis->GetRelativeRotation();

	float NewPivot = MainChassisRelativeRotation.Yaw + Val * PanSpeed;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Pivot: %f"), NewPivot));

	FRotator NewRotation(MainChassisRelativeRotation.Pitch, NewPivot, MainChassisRelativeRotation.Roll);

	MainChassis->SetRelativeRotation(NewRotation);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pivot"));
}

void ABike::ConstantForwardMovement()
{
	TObjectPtr<UPawnMovementComponent> MovementComponent = GetMovementComponent();
	if (MovementComponent)
	{
		bool IsMovingOnGround = MovementComponent->IsMovingOnGround();

				
		// TODO: Perhaps allow boosting while in air as well..
		if (IsMovingOnGround)
		{
			// assumes that we can boost
			if (IsBoosting && (Fuel > 0.0f))
			{
				Fuel -= FuelBurnRate;
				MovementSpeedModifier = 2.0f;
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fuel %f"), Fuel));
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SpeedModifier %f"), MovementSpeedModifier));
				}
			}
			else
			{
				MovementSpeedModifier = 0.5f;
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SpeedModifier %f"), MovementSpeedModifier));
			}
		}



		float Speed = DefaultMovementSpeed * MovementSpeedModifier;
		auto MainChassisLocation = MainChassis->GetComponentLocation();
		auto MainChassisRelativeRotation = MainChassis->GetRelativeRotation();
		auto MainChassisRelativeScale3D = MainChassis->GetRelativeScale3D();
		auto MainChassisTransform = MainChassis->GetComponentTransform();
		FVector SpeedVec(Speed, 0, 0);
		FVector NewDirVec = UKismetMathLibrary::TransformDirection(MainChassisTransform, SpeedVec);
		SpeedVec = NewDirVec + MainChassisLocation;
		//FTransform NewTransform(, );
		FTransform NewTransform = UKismetMathLibrary::MakeTransform(SpeedVec, MainChassisRelativeRotation, MainChassisRelativeScale3D);
		//AddMovementInput(MainChassis->GetForwardVector(), Speed);
		FVector NewLocation = NewTransform.GetLocation();
		//Continue with SetActorLocation using vector
		SetActorLocation(NewLocation, true);
	}
}

void ABike::StartBoosting()
{
	IsBoosting = true;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Is Boosting"));
}

void ABike::StopBoosting()
{
	IsBoosting = false;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Boosting"));
}

void ABike::Jump()
{
	Super::Jump();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jumping"));

	//AddMovementInput(FVector::UpVector, 300.0f);
}

void ABike::StopJumping()
{
	Super::StopJumping();
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stop Jumping"));
}

// Called every frame
void ABike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsAlive)
	{
		ConstantForwardMovement();
	}

}

// Called to bind functionality to input
void ABike::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Boosting 
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Started, this, &ABike::StartBoosting);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Completed, this, &ABike::StopBoosting);

		// Turning by 90 degrees
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Started, this, &ABike::Turn);

		// Pivoting, this gives ability to swerve, not just simply turn by 90 degrees
		EnhancedInputComponent->BindAction(PivotAction, ETriggerEvent::Triggered, this, &ABike::Pivot);
	}
}

