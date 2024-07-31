// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateBrush.h"
#include "GameFramework/Character.h"
#include "Bike.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UArrowComponent;
class UBoxComponent;
class UPawnMovementComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class AWall;
struct FInputActionValue;
struct FSlateBrush;

UCLASS()
class OUTOFSPACETORUN_API ABike : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmMinimap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* Minimap;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	UPawnMovementComponent* MovementComponent;

	// TODO: Move to component later??
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DefaultMovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeedModifier;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float PanSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsAlive;
	UPROPERTY(EditAnywhere, Category = "State")
	bool IsBoosting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mechanics")
	float CurrentFuel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mechanics")
	float MaxFuel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mechanics")
	float FuelBurnRate;

	UPROPERTY()
	FTimerHandle WallSpawnTimerHandle;

	UPROPERTY()
	FTimerHandle BoostRestoreTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostRestoreTimeInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FuelRestoreValue;

	// TODO: Had to uncomment in build file "Slate", perhaps there is a better way to paint Minimap, take a look later
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	FSlateBrush MinimapBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* MinimapTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpawnWallDistanceThreshold;
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
	void SpawnWall();
	void RestoreFuel();
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void PossessedBy(AController* NewController);

	UFUNCTION()
	void OnBoxOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWall> WallToSpawn;

	UPROPERTY(EditAnywhere)
	float DistanceTravelled;

	UPROPERTY(EditAnywhere)
	FVector PreviousPosition;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
