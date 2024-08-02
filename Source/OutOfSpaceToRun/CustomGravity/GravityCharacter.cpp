// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/CustomGravity/GravityCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);
// Sets default values
AGravityCharacter::AGravityCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsOutward = false;
	IsDebug = false;
	LerpAlpha = 0.1;
	TargetGravityDirection = FVector::DownVector;
	//InteractDistance = 500.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

}

// Called when the game starts or when spawned
void AGravityCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGravityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		if (TargetGravityActor != nullptr)
		{
			auto actor_location = GetActorLocation();
			FVector target_location;
			// For Cylinder objects there is a special case
			int direction_modifier = 1;
			if (IsOutward)
			{
				// Not sure which way is faster: FMath of UKismetMathLibrary, choose one
				/*target_location = FMath::ClosestPointOnInfiniteLine(TargetGravityActor->GetActorLocation() - TargetGravityActor->GetActorUpVector() * 10000.0f,
					TargetGravityActor->GetActorLocation() + TargetGravityActor->GetActorUpVector() * 10000.0f,
					actor_location);*/
					// We want it to go outwards
				direction_modifier = -1;
				target_location = UKismetMathLibrary::FindClosestPointOnLine(actor_location, TargetGravityActor->GetActorLocation(), TargetGravityActor->GetActorUpVector());
			}
			else
			{
				target_location = TargetGravityActor->GetActorLocation();
			}
			TargetGravityDirection = (target_location - actor_location) * direction_modifier;
			TargetGravityDirection.Normalize();
		}

		// This is a bandaid: otherwise character won't turn
		if (TargetGravityDirection == FVector::UpVector)
		{
			LerpAlpha = 1.0f;
		}
		MovementComponent->SetGravityDirection(FMath::Lerp(MovementComponent->GetGravityDirection(), TargetGravityDirection, LerpAlpha));
	}
}

void AGravityCharacter::SetTargetGravity(const FVector direction)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		//MovementComponent->SetGravityDirection(direction);
		TargetGravityDirection = direction;
	}
}

void AGravityCharacter::SetTargetGravityActor(AActor* actor, bool is_outward)
{
	TargetGravityActor = actor;
	IsOutward = is_outward;
	// Currently used to reset gravity to downward. 
	// Perhaps in near future it should use saved previous_gravity_direction to revert on exit, but there could be some issues with that as well
	if (!actor)
	{
		SetTargetGravity(FVector::DownVector);
	}
}

AActor* AGravityCharacter::GetTargetGravityActor()
{
	return TargetGravityActor;
}
