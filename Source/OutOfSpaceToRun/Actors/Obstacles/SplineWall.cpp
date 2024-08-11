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

}

// Called when the game starts or when spawned
void ASplineWall::BeginPlay()
{
	Super::BeginPlay();
	
}

//void ASplineWall::Init()
//{
//	//Super::OnConstruction(Transform);
//
//	int SplinePointsCount = Spline->GetNumberOfSplinePoints();
//
//	// Substracting 2 because we: -1 for regular last element, -1 because we want number of meshes between
//	for (int i = 0; i < SplinePointsCount - 2; i++)
//	{
//		SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
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
//
//			SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
//			AddInstanceComponent(SplineMesh);
//		}
//	}
//}

// Called every frame
void ASplineWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineWall::UpdateSplinePoint(const FTransform& Transform, bool IsNewPoint)
{
	int SplinePointsCount = Spline->GetNumberOfSplinePoints();
	if (IsNewPoint)
	{
		//AddPoint
		Spline->AddSplinePointAtIndex(Transform.GetLocation(), SplinePointsCount, ESplineCoordinateSpace::Local);
		SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (SplineMesh)
		{
			SplineMesh->SetStaticMesh(WallMesh);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
			SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->RegisterComponent();
			SplineMesh->SetMaterial(0, MaterialRef);
			FVector StartPosition;
			FVector StartTangent;
			FVector EndPosition;
			FVector EndTangent;
			if (SplinePointsCount > 2)
			{
				Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 2, StartPosition, StartTangent);
				Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 1, EndPosition, EndTangent);
				SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
			}
			AddInstanceComponent(SplineMesh);
		}
	}
	else
	{
		if (SplinePointsCount < 2)
			return;
		// Otherwise take last point and update its position
		FSplinePoint SplinePoint = Spline->GetSplinePointAt(SplinePointsCount - 1, ESplineCoordinateSpace::Local);
		FVector StartPosition;
		FVector StartTangent;
		FVector EndPosition;
		FVector EndTangent;
		Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 2, StartPosition, StartTangent);
		Spline->GetLocalLocationAndTangentAtSplinePoint(SplinePointsCount - 1, EndPosition, EndTangent);

		FSplinePoint PreviousSplinePoint = Spline->GetSplinePointAt(SplinePointsCount - 1, ESplineCoordinateSpace::Local);
		SplinePoint.Position = Transform.GetLocation();
		Spline->SetLocationAtSplinePoint(SplinePointsCount - 1, Transform.GetLocation(), ESplineCoordinateSpace::Local);
		SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
	}
}

