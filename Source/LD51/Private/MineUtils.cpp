// Fill out your copyright notice in the Description page of Project Settings.


#include "MineUtils.h"
#include "MineField.h"
#include "Cell.h"

TArray<ACell*> UMineUtils::GetNeighboringCellsByCell(
	AMineField* Field, ACell* Cell, bool bIncludeSelf)
{
	if (!IsValid(Field))
	{
		return TArray<ACell*>();
	}

	if (!IsValid(Cell))
	{
		return TArray<ACell*>();
	}

	FGridCoords CellCoords = Field->GetCellCoordinates(Cell);

	return GetNeighboringCellsByCoords(Field, CellCoords, bIncludeSelf);
}

TArray<ACell*> UMineUtils::GetNeighboringCellsByCoords(
	const AMineField* Field, const FGridCoords& CellCoords, bool bIncludeSelf)
{
	if (!IsValid(Field))
	{
		return TArray<ACell*>();
	}

	TArray<FGridCoords> NeighboringCoords = GetNeighboringCoords(CellCoords, bIncludeSelf);

	return GetCellsByCoords(Field, NeighboringCoords);
}

TArray<FGridCoords> UMineUtils::GetNeighboringCoords(
	const FGridCoords& CellCoords, bool bIncludeSelf)
{
	TArray<FGridCoords> Result;

	TArray<FGridCoords> Directions =
	{
		FGridCoords(1, 0),
		FGridCoords(-1, 0),
		FGridCoords(0, 1),
		FGridCoords(0, -1),
		FGridCoords(1, 1),
		FGridCoords(-1, -1),
		FGridCoords(1, -1),
		FGridCoords(-1, 1)
	};

	for (const FGridCoords& Direction : Directions)
	{
		FGridCoords Cell = CellCoords + Direction;
		Result.Add(Cell);
	}

	return Result;
}

TArray<ACell*> UMineUtils::GetCellsByCoords(
	const AMineField* Field, const TArray<FGridCoords>& CellCoords)
{
	TArray<ACell*> Result;

	if (!IsValid(Field))
	{
		return Result;
	}

	Result.Reserve(CellCoords.Num());

	for (const FGridCoords& Coords : CellCoords)
	{
		ACell* Cell = Field->GetCellByCoords(Coords);
		if (IsValid(Cell))
		{
			Result.Add(Cell);
		}
	}

	return Result;
}

TArray<FGridCoords> UMineUtils::GetRegionCoords(const FGridCoords& From, const FGridCoords& To)
{
	TArray<FGridCoords> Result;

	int StartRow = FMath::Min(From.Row, To.Row);
	int EndRow = FMath::Max(From.Row, To.Row);
	int StartCol = FMath::Min(From.Column, To.Column);
	int EndCol = FMath::Max(From.Column, To.Column);

	for (int Row = StartRow; Row <= EndRow; ++Row)
	{
		for (int Column = StartCol; Column <= EndCol; ++Column)
		{
			Result.Add(FGridCoords(Column, Row));
		}
	}

	return Result;
}
