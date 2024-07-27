// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bike.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UArrowComponent;
struct FInputActionValue;

UCLASS()
class OUTOFSPACETORUN_API ABike : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmMinimap;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TurnAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PivotAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BoostAction;

protected:

	// This one is taken from BP I've used previously, don't recall why
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MainChassis;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Engine;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HandleBars;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FeetSupport;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WheelsGuards;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Wheels;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Roof;

	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow;


	// TODO: Move to component later??
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DefaultMovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeedModifier;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float PanSpeed;

	UPROPERTY(EditAnywhere, Category = "State")
	bool IsAlive;
	UPROPERTY(EditAnywhere, Category = "State")
	bool IsBoosting;

	UPROPERTY(EditAnywhere, Category = "VectorsRotators")
	FVector PreviousPosition;
	UPROPERTY(EditAnywhere, Category = "VectorsRotators")
	FVector PreviousForwardVector;
	UPROPERTY(EditAnywhere, Category = "VectorsRotators")
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = "Mechanics")
	float Fuel;
	UPROPERTY(EditAnywhere, Category = "Mechanics")
	float FuelBurnRate;

public:
	// Sets default values for this character's properties
	ABike();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/** Called for turn input */
	void Turn(const FInputActionValue& Value);
	/** Called for pivot input */
	void Pivot(const FInputActionValue& Value);

	void ConstantForwardMovement();
	void StartBoosting();
	void StopBoosting();
	virtual void Jump() override;
	virtual void StopJumping() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
