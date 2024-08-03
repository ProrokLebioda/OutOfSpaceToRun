// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/UI/HUD/UIBoost.h"
#include "OutOfSpaceToRun/Actors/Controllers/BikePlayerController.h"
#include "OutOfSpaceToRun/Actors/Bike/Bike.h"
#include "UIBoost.h"

void UUIBoost::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PlayerController = GetOwningPlayer();
	if (auto BikeController = Cast<ABikePlayerController>(PlayerController))
	{
		
		if (auto BikePawn = Cast<ABike>(BikeController->GetPawn()))
		{
			PlayerPawn = BikePawn;
		}
	}
}

float UUIBoost::GetBoostBarPercent()
{
	float CurrentFuel = PlayerPawn->GetCurrentFuel();
	float MaxFuel = PlayerPawn->GetMaxFuel();

	float BoostBarPercentage = CurrentFuel / MaxFuel;

	return BoostBarPercentage;
}
