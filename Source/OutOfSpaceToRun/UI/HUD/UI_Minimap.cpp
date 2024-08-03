// Fill out your copyright notice in the Description page of Project Settings.


#include "OutOfSpaceToRun/UI/HUD/UI_Minimap.h"
#include "Components/Image.h"
#include "UI_Minimap.h"

FSlateBrush UUI_Minimap::GetMinimapImageBrush()
{
	return BrushMinimapMaterial;
}

void UUI_Minimap::SetMinimapImageBrush(FSlateBrush NewSlateBrush)
{
	BrushMinimapMaterial = NewSlateBrush;
}
