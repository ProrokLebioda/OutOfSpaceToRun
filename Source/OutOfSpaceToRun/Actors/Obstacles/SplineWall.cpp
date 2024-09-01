// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/Actors/Obstacles/SplineWall.h"
#include "SplineWall.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineMeshComponent.h"


// Sets default values
ASplineWall::ASplineWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TopScene = CreateDefaultSubobject<USceneComponent>(TEXT("Top Scene"));
	RootComponent = TopScene;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(TopScene);

	SplineMeshClass = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Test Spline Mesh"));
	SplineMeshClass->SetupAttachment(Spline);
}

// Called when the game starts or when spawned
void ASplineWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASplineWall::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateSplineMeshes();
}

//void ASplineWall::OnConstruction(const FTransform& Transform)
//{
//	Super::OnConstruction(Transform);
//	SplineMesh = nullptr;
//	SplineMeshArray.Empty();
//
//	int SplinePointsCount = Spline->GetNumberOfSplinePoints();
//
//	// Substracting 2 because we: -1 for regular last element, -1 because we want number of meshes between
//	for (int i = 0; i < SplinePointsCount - 2; i++)
//	{
//		if (!SplineMesh)
//		{
//			SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
//		}
//		
//		if (SplineMesh)
//		{
//			SplineMesh->SetStaticMesh(WallMesh);
//			SplineMesh->SetMobility(EComponentMobility::Movable);
//			SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
//			SplineMesh->AttachToComponent(TopScene, FAttachmentTransformRules::KeepRelativeTransform);
//			SplineMesh->RegisterComponent();
//			SplineMesh->SetMaterial(0, MaterialRef);
//			FVector StartPosition;
//			FVector StartTangent;
//			FVector EndPosition;
//			FVector EndTangent;
//			Spline->GetLocalLocationAndTangentAtSplinePoint(i, StartPosition, StartTangent);
//			Spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPosition, EndTangent);
//			Spline->AddPoint()
//			SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
//			SplineMeshArray.Add(SplineMesh);
//			AddInstanceComponent(SplineMesh);
//			
//		}
//	}
//}

// Called every frame
void ASplineWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASplineWall::UpdateSplineMeshes()
{
	for (USplineMeshComponent* SplineMesh2 : SplineMeshesArray)
	{
		if (SplineMesh2)
		{
			SplineMesh2->DestroyComponent();
		}
	}

	SplineMeshesArray.Empty();

	// Create new spline meshes

	const int32 NumSegments = Spline->GetNumberOfSplinePoints() - 1;
	for (int32 i = 0; i < NumSegments; i++)
	{
		//Create new spline mesh component
		if (SplineMeshClass)
		{
			USplineMeshComponent* SplineMesh2 = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

			if (SplineMesh2)
			{
				SplineMesh2->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMesh2->RegisterComponent();
				SplineMeshesArray.Add(SplineMesh2);

				//Set start and end positions and tangents
				FVector StartPos, StartTangent, EndPos, EndTangent;
				Spline->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
				Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

				SplineMesh2->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);

				// Set mesh material or any other properties here
				// SplineMesh->SetStaticMesh(SomeStaticMesh);
				// SplineMesh->SetMaterial(0, SomeMaterial);
			}
		}
	}
}

void ASplineWall::AddSplinePoint(const FVector& Location)
{
	Spline->AddSplinePoint(Location, ESplineCoordinateSpace::Local, true);
	UpdateSplineMeshes();
}

//void ASplineWall::UpdateSplineMeshes()
//{
//	int SplinePointsCount = Spline->GetNumberOfSplinePoints();
//	{
//		//AddPoint
//		Spline->AddSplinePointAtIndex(Transform.GetLocation(), SplinePointsCount, ESplineCoordinateSpace::World);
//
//		SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
//		if (SplineMesh)
//		{
//			SplineMesh->SetStaticMesh(WallMesh);
//			SplineMesh->SetMobility(EComponentMobility::Movable);
//			SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
//			SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
//			SplineMesh->RegisterComponent();
//			SplineMesh->SetMaterial(0, MaterialRef);
//			FVector StartPosition;
//			FVector StartTangent;
//			FVector EndPosition;
//			FVector EndTangent;
//			if (SplinePointsCount > 2)
//			{
//				Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 2, StartPosition, StartTangent);
//				Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 1, EndPosition, EndTangent);
//				SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
//				SplineMesh->SetForwardAxis(ESplineMeshAxis::X);
//				SplineMeshArray.Add(SplineMesh);
//				
//			}
//			AddInstanceComponent(SplineMesh);
//		}
//	}
//	else
//	{
//		if (!SplineMesh)
//			return;
//		if (SplinePointsCount < 2)
//			return;
//		// Otherwise take last point and update its position
//		FSplinePoint SplinePoint = Spline->GetSplinePointAt(SplinePointsCount - 1, ESplineCoordinateSpace::World);
//		FVector StartPosition;
//		FVector StartTangent;
//		FVector EndPosition;
//		FVector EndTangent;
//		Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 2, StartPosition, StartTangent);
//		Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 1, EndPosition, EndTangent);
//
//		FSplinePoint PreviousSplinePoint = Spline->GetSplinePointAt(SplinePointsCount - 1, ESplineCoordinateSpace::World);
//		SplinePoint.Position = Transform.GetLocation();
//		Spline->SetLocationAtSplinePoint(SplinePointsCount - 1, Transform.GetLocation(), ESplineCoordinateSpace::World);
//		SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
//	}
//}

