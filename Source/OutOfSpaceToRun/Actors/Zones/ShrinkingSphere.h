// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShrinkingSphere.generated.h"

class UStaticMeshComponent;

UCLASS()
class OUTOFSPACETORUN_API AShrinkingSphere : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Sphere;
	
	UPROPERTY(EditAnywhere)
	float ShrinkSpeed;

	UPROPERTY(EditAnywhere)
	float MinimumScale3D;

public:	
	// Sets default values for this actor's properties
	AShrinkingSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
