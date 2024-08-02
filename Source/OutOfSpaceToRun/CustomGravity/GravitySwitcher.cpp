// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/CustomGravity/GravitySwitcher.h"

// Sets default values
AGravitySwitcher::AGravitySwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AGravitySwitcher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravitySwitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

