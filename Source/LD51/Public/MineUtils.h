// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridStructs.h"
#include "MineUtils.generated.h"

class AMineField;
class ACell;

UCLASS()
class LD51_API UMineUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<ACell*> GetNeighboringCellsByCell(AMineField* Field, ACell* Cell, bool bIncludeSelf = false);

	UFUNCTION(BlueprintCallable)
	static TArray<ACell*> GetNeighboringCellsByCoords(const AMineField* Field, const FGridCoords& CellCoords, bool bIncludeSelf = false);

	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetNeighboringCoords(const FGridCoords& CellCoords, bool bIncludeSelf = false);

	UFUNCTION(BlueprintCallable)
	static TArray<ACell*> GetCellsByCoords(const AMineField* Field, const TArray<FGridCoords>& CellCoords);

	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetRegionCoords(const FGridCoords& From, const FGridCoords& To);
};
