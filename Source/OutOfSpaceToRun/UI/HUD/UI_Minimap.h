// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateBrush.h"
#include "UI_Minimap.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class OUTOFSPACETORUN_API UUI_Minimap : public UUserWidget
{
	GENERATED_BODY()

public:
	// TODO: Had to uncomment in build file "Slate", perhaps there is a better way to paint Minimap, take a look later
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FSlateBrush BrushMinimapMaterial;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MinimapImage;

public:
	UFUNCTION(BlueprintGetter)
	FSlateBrush GetMinimapImageBrush();

	UFUNCTION(BlueprintSetter)
	void SetMinimapImageBrush(FSlateBrush NewSlateBrush);
};
