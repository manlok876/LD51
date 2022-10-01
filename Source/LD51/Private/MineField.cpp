// Fill out your copyright notice in the Description page of Project Settings.


#include "MineField.h"
#include "MineUtils.h"
#include "Cell.h"
#include "Bomb.h"

FMazeSize::FMazeSize()
{
	Columns = Rows = 0;
}

FMazeSize::FMazeSize(int NumColumns, int NumRows)
{
	Columns = NumColumns;
	Rows = NumRows;
}

AMineField::AMineField()
{
	PrimaryActorTick.bCanEverTick = true;

	CellClass = ACell::StaticClass();

	bInitialized = false;
	Width = 10;
	Length = 10;
}

void AMineField::BeginPlay()
{
	Super::BeginPlay();

	Init(Width, Length);
}

void AMineField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMineField::Init_Implementation(int W, int L)
{
	Width = 0;
	Length = 0;

	UpdateMazeSize(W, L);

	bInitialized = true;
}

void AMineField::HandleCellAdded_Implementation(ACell* NewCell)
{
}

void AMineField::UpdateCell_Implementation(ACell* CellToUpdate)
{
}

int AMineField::GetLength() const
{
	return Length;
}

void AMineField::SetLength(int NewLength)
{
	UpdateMazeSize(Width, NewLength);
}

int AMineField::GetWidth() const
{
	return Width;
}

void AMineField::SetWidth(int NewWidth)
{
	UpdateMazeSize(NewWidth, Length);
}

FMazeSize AMineField::GetMazeSize() const
{
	return FMazeSize(Width, Length);
}

void AMineField::SetSize(const FMazeSize& NewSize)
{
	UpdateMazeSize(NewSize.Columns, NewSize.Rows);
}

void AMineField::UpdateMazeSize(int NewWidth, int NewLength)
{
	if (NewWidth == Width && NewLength == Length)
	{
		return;
	}
	int OldWidth = Width;
	int OldLength = Length;
	TArray<ACell*> OldCells = Cells;

	Width = NewWidth;
	Length = NewLength;
	Cells.SetNumZeroed(NewWidth * NewLength);

	TArray<ACell*> NewCells;

	TArray<ACell*> RemovedCells;

	TArray<ACell*> RetainedCells;

	for (int CellX = 0; CellX < NewWidth; ++CellX)
	{
		for (int CellY = 0; CellY < NewLength; ++CellY)
		{
			const int CellIdx = GetCellIndex1D(CellX, CellY);
			check(Cells.IsValidIndex(CellIdx));
			ACell* NewCell = nullptr;

			if (CellX < OldWidth && CellY < OldLength)
			{
				const int OldCellIdx =
					Index1DFromIndex2D(CellX, OldWidth, CellY, OldLength);
				check(OldCells.IsValidIndex(OldCellIdx));
				NewCell = OldCells[OldCellIdx];
				OldCells[OldCellIdx] = nullptr;
				RetainedCells.Add(NewCell);
			}
			else
			{
				NewCell = GetWorld()->SpawnActor<ACell>(CellClass);
				NewCells.Add(NewCell);
			}

			Cells[CellIdx] = NewCell;
		}
	}

	for (ACell* OldCell : OldCells)
	{
		if (OldCell != nullptr)
		{
			RemovedCells.Add(OldCell);
		}
	}

	// Update old cells/walls positions etc.
	for (ACell* RetainedCell : RetainedCells)
	{
		UpdateCell(RetainedCell);
	}

	// Handle new elements being added
	for (ACell* NewCell : NewCells)
	{
		HandleCellAdded(NewCell);
	}

	for (ACell* OldCell : RemovedCells)
	{
		if (IsValid(OldCell))
		{
			OldCell->Destroy();
		}
	}

}

bool AMineField::IsValidCell(int Column, int Row) const
{
	return Cells.IsValidIndex(GetCellIndex1D(Column, Row));
}

bool AMineField::ContainsCell(ACell* Cell) const
{
	if (!IsValid(Cell))
	{
		return nullptr;
	}

	return Cells.Contains(Cell);
}

ACell* AMineField::GetCell(int Column, int Row) const
{
	int CellIdx1D = GetCellIndex1D(Column, Row);
	if (!Cells.IsValidIndex(CellIdx1D))
	{
		return nullptr;
	}
	return Cells[CellIdx1D];
}

ACell* AMineField::GetCellByCoords(const FGridCoords& Coordinates) const
{
	return GetCell(Coordinates.Column, Coordinates.Row);
}

TArray<ACell*> AMineField::GetAllCells() const
{
	return Cells;
}

FGridCoords AMineField::GetCellCoordinates(ACell* Cell)
{
	int CellIdx1D = Cells.Find(Cell);
	if (!Cells.IsValidIndex(CellIdx1D))
	{
		return FGridCoords();
	}

	int Row, Column;
	GetCellIndex2D(CellIdx1D, Column, Row);
	check(Column >= 0 && Row >= 0);

	return FGridCoords(Column, Row);
}

bool AMineField::GetNeighboringCells(ACell* TargetCell)
{
	if (!IsValid(TargetCell))
	{
		return false;
	}

	if (!ContainsCell(TargetCell))
	{
		return false;
	}

	FGridCoords TargetCellCoords = GetCellCoordinates(TargetCell);
	int Row = TargetCellCoords.Row;
	int Column = TargetCellCoords.Column;
	// Already checked that this cell is in this maze
	check(Row >= 0 && Column >= 0);

	return true;
}

void AMineField::CoverRegion(const FGridCoords& From, const FGridCoords& To)
{
	CoverCoords(UMineUtils::GetRegionCoords(From, To));
}

void AMineField::CoverCoords(const TArray<FGridCoords>& InCoords)
{
	for (const FGridCoords& Coords : InCoords)
	{
		ACell* Cell = GetCellByCoords(Coords);
		if (IsValid(Cell))
		{
			if (Cell->IsOpen())
			{
				Cell->CoverCell();
			}
		}
	}
}

TArray<ACell*> AMineField::GetAllFlagCells() const
{
	TArray<ACell*> Result;

	int CellColumn, CellRow;
	for (ACell* Cell : Cells)
	{
		if (IsValid(Cell) && Cell->IsFlagged())
		{
			Result.Add(Cell);
		}
	}

	return Result;
}

TArray<FGridCoords> AMineField::GetAllFlagCoords() const
{
	TArray<FGridCoords> Result;

	int CellColumn, CellRow;
	for (int CellIdx1D = 0; CellIdx1D < Cells.Num(); ++CellIdx1D)
	{
		ACell* Cell = Cells[CellIdx1D];
		if (IsValid(Cell) && Cell->IsFlagged())
		{
			GetCellIndex2D(CellIdx1D, CellColumn, CellRow);
			Result.Add(FGridCoords(CellColumn, CellRow));
		}
	}

	return Result;
}

TArray<ACell*> AMineField::GetAllBombCells() const
{
	TArray<ACell*> Result;

	int CellColumn, CellRow;
	for (ACell* Cell : Cells)
	{
		if (IsValid(Cell) && Cell->HasBomb())
		{
			Result.Add(Cell);
		}
	}

	return Result;
}

TArray<FGridCoords> AMineField::GetAllBombCoords() const
{
	TArray<FGridCoords> Result;

	int CellColumn, CellRow;
	for (int CellIdx1D = 0; CellIdx1D < Cells.Num(); ++CellIdx1D)
	{
		ACell* Cell = Cells[CellIdx1D];
		if (IsValid(Cell) && Cell->HasBomb())
		{
			GetCellIndex2D(CellIdx1D, CellColumn, CellRow);
			Result.Add(FGridCoords(CellColumn, CellRow));
		}
	}

	return Result;
}

TArray<ABomb*> AMineField::GetAllBombs() const
{
	TArray<ABomb*> Result;

	int CellColumn, CellRow;
	for (ACell* Cell : Cells)
	{
		if (IsValid(Cell))
		{
			ABomb* Bomb = Cell->GetBomb();
			if (IsValid(Bomb))
			{
				Result.Add(Bomb);
			}
		}
	}

	return Result;
}

int AMineField::GetCellIndex1D(int Column, int Row) const
{
	return Index1DFromIndex2D(Column, Width, Row, Length);
}

int AMineField::Index1DFromIndex2D(int X, int SizeX, int Y, int SizeY)
{
	if (X < 0 || Y < 0 ||
		X >= SizeX || Y >= SizeY)
	{
		return -1;
	}
	return Y * SizeX + X;
}

void AMineField::GetCellIndex2D(int Idx1D, int& Column, int& Row) const
{
	Index2DFromIndex1D(Idx1D, Width, Length, Column, Row);
}

void AMineField::Index2DFromIndex1D(int Idx1D, int SizeX, int SizeY, int& X, int& Y)
{
	if (SizeX < 1 || SizeY < 1 ||
		Idx1D < 0 || Idx1D > SizeX * SizeY)
	{
		X = Y = -1;
		return;
	}
	X = Idx1D % SizeX;
	Y = Idx1D / SizeX;
}
