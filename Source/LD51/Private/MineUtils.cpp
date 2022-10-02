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

FGridCoords UMineUtils::GetRandomCoords(const AMineField* Field)
{
	if (!IsValid(Field))
	{
		return FGridCoords();
	}

	return FGridCoords(FMath::RandHelper(Field->GetMazeSize().Columns),
		FMath::RandHelper(Field->GetMazeSize().Rows));
}

FGridCoords UMineUtils::GetRandomCoordsWithoutBomb(const AMineField* Field)
{
	if (!IsValid(Field))
	{
		return FGridCoords();
	}

	FGridCoords Result = GetRandomCoords(Field);
	ACell* ResultCell = Field->GetCellByCoords(Result);
	check(IsValid(ResultCell));

	while (ResultCell->HasBomb())
	{
		Result = GetRandomCoords(Field);
		ResultCell = Field->GetCellByCoords(Result);
		check(IsValid(ResultCell));
	}

	return Result;
}

void UMineUtils::GetRegion(const FGridCoords& Center, int Diameter,
	FGridCoords& From, FGridCoords& To)
{
	if (Diameter <= 1)
	{
		From = To = Center;
	}

	int SideOffset = Diameter - 1;
	FGridCoords StartOffset(SideOffset / 2, SideOffset / 2);
	FGridCoords RegionSpan(SideOffset, SideOffset);

	From = Center + StartOffset;
	To = From + RegionSpan;
}

TArray<FGridCoords> UMineUtils::FilterValidCoordsForField(const AMineField* Field, const TArray<FGridCoords>& Coords)
{
	if (!IsValid(Field))
	{
		return TArray<FGridCoords>();
	}

	TArray<FGridCoords> Result = Coords.FilterByPredicate(
		[&] (const FGridCoords& CellCoords) {
			return Field->IsValidCoords(CellCoords);
		}
	);

	return Result;
}

FGridCoords UMineUtils::GetRandomFieldCoordsInRegion(const AMineField* Field, const FGridCoords& Center, int Diameter)
{
	if (!IsValid(Field))
	{
		return Center;
	}

	if (Diameter <= 1)
	{
		return Center;
	}

	FGridCoords RegFrom, RegTo;
	GetRegion(Center, Diameter, RegFrom, RegTo);
	TArray<FGridCoords> AllRegCoords = GetRegionCoords(RegFrom, RegTo);

	TArray<FGridCoords> FilteredCoords = FilterValidCoordsForField(Field, AllRegCoords);

	if (FilteredCoords.Num() < 1)
	{
		return Center;
	}

	int RandomCoordsIdx = FMath::RandHelper(FilteredCoords.Num());

	return FilteredCoords[RandomCoordsIdx];
}
