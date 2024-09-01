// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineWall.generated.h"

class USceneComponent;
class USplineComponent;
class USplineMeshComponent;

UCLASS()
class OUTOFSPACETORUN_API ASplineWall : public AActor
{
	GENERATED_BODY()
	


public:	
	// Sets default values for this actor's properties
	ASplineWall();
	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialRef;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	/*UFUNCTION(BlueprintCallable)
	void Init();*/
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* WallMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateSplineMeshes();

	void AddSplinePoint(const FVector& Location);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USplineMeshComponent* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USplineMeshComponent*> SplineMeshesArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SplineMesh")
	USplineMeshComponent* SplineMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* TopScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;
};
