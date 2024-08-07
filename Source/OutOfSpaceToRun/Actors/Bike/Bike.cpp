// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/Actors/Bike/Bike.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Kismet/KismetMathLibrary.h>
#include "OutOfSpaceToRun/Actors/Obstacles/Wall.h"
#include "OutOfSpaceToRun/Actors/Zones/ShrinkingSphere.h"
#include "OutOfSpaceToRun/Actors/Controllers/BikePlayerController.h"

// Sets default values
ABike::ABike()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Defaults

	MaxFuel = 100.0f;
	CurrentFuel = MaxFuel;
	FuelBurnRate = 1.0f;
	IsAlive = true;
	IsBoosting = false;
	DefaultMovementSpeed = 50.0f;
	MovementSpeedModifier = 1.0f;
	PanSpeed = 5.0f;
	DistanceTravelled = 0.0f;
	SpawnWallDistanceThreshold = 20.0f;
	BoostRestoreTimeInterval = 0.2f;
	FuelRestoreValue = 1.0f;
	
	
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

	// Used for most collisions
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(MainChassis);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABike::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ABike::OnBoxEndOverlap);

	//// Setup parameters in editor
	SpringArmMinimap = CreateDefaultSubobject<USpringArmComponent>(TEXT("Minimap Spring Arm"));
	SpringArmMinimap->SetupAttachment(MainChassis);

	Minimap = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Minimap"));
	Minimap->SetupAttachment(SpringArmMinimap);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Player Camera"));
	CameraBoom->SetupAttachment(MainChassis);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
}

// Called when the game starts or when spawned
void ABike::BeginPlay()
{
	Super::BeginPlay();
	
	// Move to a timer
	MovementComponent = GetMovementComponent();
	
	// TODO: Commented out in favor of solution that checks distance travelled
	// Start timer spawning wall
	//GetWorld()->GetTimerManager().SetTimer(
	//	WallSpawnTimerHandle, // handle to cancel timer at a later time
	//	this, // the owning object
	//	&ABike::SpawnWall, // function to call on elapsed
	//	0.2f, // float delay until elapsed
	//	true); // looping?
	

	PreviousPosition = GetActorLocation();

}

// Turns vehicle by 90 degrees
void ABike::Turn(const FInputActionValue& Value)
{
	if (!IsAlive)
		return;
	
	//TObjectPtr<UPawnMovementComponent> MovementComponent = GetMovementComponent();
	//if (!MovementComponent)
	//	return;

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

	//TObjectPtr<UPawnMovementComponent> MovementComponent = GetMovementComponent();
	//if (!MovementComponent)
	//	return;

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
	if (MovementComponent)
	{
		bool IsMovingOnGround = MovementComponent->IsMovingOnGround();
				
		// TODO: Perhaps allow boosting while in air as well..
		if (IsMovingOnGround)
		{
			// assumes that we can boost
			if (IsBoosting && (CurrentFuel > 0.0f))
			{
				CurrentFuel -= FuelBurnRate;
				MovementSpeedModifier = 2.0f;
			}
			else
			{
				MovementSpeedModifier = 0.5f;
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
	if (BoostRestoreTimerHandle.IsValid())
	{
		BoostRestoreTimerHandle.Invalidate();
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Is Boosting"));
}

void ABike::StopBoosting()
{
	IsBoosting = false;
	if (!BoostRestoreTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(
			BoostRestoreTimerHandle, // handle to cancel timer at a later time
			this, // the owning object
			&ABike::RestoreFuel, // function to call on elapsed
			BoostRestoreTimeInterval, // float delay until elapsed
			true); // looping?
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Boosting"));
}

void ABike::SpawnWall()
{
	auto Location = MainChassis->GetComponentLocation();
	const auto Rotation = MainChassis->GetComponentRotation();
	FVector PositionOffset = MainChassis->GetForwardVector() * -150.f;
	Location = Location + PositionOffset;

	GetWorld()->SpawnActor<AActor>(WallToSpawn, Location, Rotation);
}

void ABike::Jump()
{
	if (!IsAlive)
		return;
	Super::Jump();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jumping"));
}

void ABike::StopJumping()
{
	Super::StopJumping();
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stop Jumping"));
}

void ABike::OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWall* Wall = Cast<AWall>(OtherActor);
	if (Wall)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->SetActive(false);
		IsAlive = false;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlapp Begin - Wall"));
	}
}

void ABike::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (APawn* Pawn = NewController->GetPawn())
	{
		if (ABikePlayerController* BikeController = Cast< ABikePlayerController>(GetController()))
		{
			int32 ControllerId = BikeController->GetLocalPlayer()->GetControllerId();
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Controller ID: %i"), ControllerId));
		}
	}
		
}

// Called every frame
void ABike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsAlive)
	{
		ConstantForwardMovement();

		FVector CurrentPosition = GetActorLocation();
		// Spawn a wall if current distance travelled threshold is exceeded, then reset threshold
		float Distance = FVector::Dist(PreviousPosition, CurrentPosition);
		DistanceTravelled += Distance;
		if (DistanceTravelled >= SpawnWallDistanceThreshold)
		{
			SpawnWall();
			DistanceTravelled = 0.0f;
		}

		PreviousPosition = CurrentPosition;
	}

}

void ABike::RestoreFuel()
{
	CurrentFuel += FuelRestoreValue;
	if (CurrentFuel >= MaxFuel)
	{
		if (BoostRestoreTimerHandle.IsValid())
		{
			BoostRestoreTimerHandle.Invalidate();
		}
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

void ABike::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShrinkingSphere* ShrinkingSphere = Cast<AShrinkingSphere>(OtherActor);
	if (ShrinkingSphere)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->SetActive(false);
		IsAlive = false;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlapp End - Shrinking Sphere"));
	}

}

