// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GravityCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class OUTOFSPACETORUN_API AGravityCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpAlpha;

	UPROPERTY(EditAnywhere)
	AActor* TargetGravityActor;

	UPROPERTY(EditAnywhere)
	FVector TargetGravityDirection;

	UPROPERTY(EditAnywhere)
	bool IsOutward;

	UPROPERTY(EditAnywhere)
	bool IsDebug;

public:
	// Sets default values for this character's properties
	AGravityCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTargetGravity(const FVector direction);

	UFUNCTION(BlueprintCallable)
	void SetTargetGravityActor(AActor* actor, bool is_outward);
	UFUNCTION(BlueprintCallable)
	AActor* GetTargetGravityActor();
};
