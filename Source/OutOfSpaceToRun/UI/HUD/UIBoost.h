// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBoost.generated.h"

class UProgressBar;
class ABike;
/**
 * 
 */
UCLASS()
class OUTOFSPACETORUN_API UUIBoost : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	ABike* PlayerPawn;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* BoostBar;
public:
	UFUNCTION(BlueprintGetter)
	float GetBoostBarPercent();
};
