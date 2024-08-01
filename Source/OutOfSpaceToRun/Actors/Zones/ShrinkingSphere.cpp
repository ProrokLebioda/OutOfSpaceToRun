// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/Actors/Zones/ShrinkingSphere.h"
#include "Components/SphereComponent.h"

// Sets default values
AShrinkingSphere::AShrinkingSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShrinkSpeed = 50.f;
	MinimumScale3D = 50.f;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
}

// Called when the game starts or when spawned
void AShrinkingSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShrinkingSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector SphereScale3D = Sphere->GetRelativeScale3D();

	// This assumes equal shrinking in all directions
	float ShrinkValue = ShrinkSpeed * DeltaTime;
	
	FVector ScaleDecrease{ ShrinkValue, ShrinkValue, ShrinkValue };

	FVector NewScale3D = SphereScale3D - ScaleDecrease;


	if (NewScale3D.X > MinimumScale3D && NewScale3D.Y > MinimumScale3D && NewScale3D.Z > MinimumScale3D)
	{
		Sphere->SetRelativeScale3D(NewScale3D);
	}


}

