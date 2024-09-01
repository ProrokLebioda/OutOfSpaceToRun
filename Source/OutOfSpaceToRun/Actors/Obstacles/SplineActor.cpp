// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineActor.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	bool retFlag;
	UpdateSplineComponent(retFlag);
	if (retFlag) return;
}

void ASplineActor::UpdateSplineComponent()
{
	for (USplineMeshComponent* SplineMesh2 : SplineMeshesArray)
	{
		if (SplineMesh2)
		{
			SplineMesh2->DestroyComponent();
		}
	}

	SplineMeshesArray.Empty();

	if (!Mesh)
	{
		return;
	}

	for (int SplineCount = 0; SplineCount < (SplineComponent->GetNumberOfSplinePoints()) - 1; SplineCount++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		SplineMeshesArray.Add(SplineMeshComponent);

		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		SplineMeshComponent->SetForwardAxis(ForwardAxis);

		if (AlternativeMaterial && SplineCount > 0 && SplineCount % 2 == 0)
		{
			SplineMeshComponent->SetMaterial(0, AlternativeMaterial);
		}
		else if (DefaultMaterial)
		{
			SplineMeshComponent->SetMaterial(0, DefaultMaterial);
		}
	}
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineActor::AddSplinePoint(const FVector& Location)
{
	int32 LastSplineIndex = SplineComponent->GetNumberOfSplinePoints() - 1;

	SplineComponent->AddSplinePointAtIndex(Location, LastSplineIndex + 1, ESplineCoordinateSpace::Local, true);
	UpdateSplineComponent();
}

void ASplineActor::UpdateLastSplinePoint(const FVector& Location)
{
	int32 LastSplineIndex = SplineComponent->GetNumberOfSplinePoints() - 1;

	FSplinePoint SplinePoint = SplineComponent->GetSplinePointAt(LastSplineIndex, ESplineCoordinateSpace::Local);

	SplinePoint.Position = Location;
	
	UpdateSplineComponent();
}

