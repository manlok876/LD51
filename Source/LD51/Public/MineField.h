// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridStructs.h"
#include "MineField.generated.h"

class ACell;
class ABomb;

USTRUCT(BlueprintType)
struct FMazeSize
{
	GENERATED_BODY()

public:
	FMazeSize();
	FMazeSize(int NumColumns, int NumRows);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Columns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Rows;
};


UCLASS(BlueprintType, Blueprintable)
class LD51_API AMineField : public AActor
{
	GENERATED_BODY()

public:
	AMineField();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACell> CellClass;

	int GetLength() const;
	UFUNCTION(BlueprintCallable)
	void SetLength(int NewLength);

	int GetWidth() const;
	UFUNCTION(BlueprintCallable)
	void SetWidth(int NewWidth);

	UFUNCTION(BlueprintPure)
	FMazeSize GetMazeSize() const;
	UFUNCTION(BlueprintCallable)
	void SetSize(const FMazeSize& NewSize);

	UFUNCTION(BlueprintPure)
	bool IsValidCell(int Column, int Row) const;
	UFUNCTION(BlueprintPure)
	bool IsValidCoords(const FGridCoords& Coordinates) const;
	UFUNCTION(BlueprintPure)
	bool ContainsCell(ACell* Cell) const;
	UFUNCTION(BlueprintPure)
	ACell* GetCell(int Column, int Row) const;
	UFUNCTION(BlueprintPure)
	ACell* GetCellByCoords(const FGridCoords& Coordinates) const;
	UFUNCTION(BlueprintPure)
	FGridCoords GetCellCoordinates(ACell* Cell);
	UFUNCTION(BlueprintCallable)
	TArray<ACell*> GetAllCells() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool GetNeighboringCells(ACell* TargetCell);

	UFUNCTION(BlueprintCallable)
	void CoverRegion(const FGridCoords& From, const FGridCoords& To);
	UFUNCTION(BlueprintCallable)
	void CoverCoords(const TArray<FGridCoords>& InCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<ACell*> GetAllFlagCells() const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<FGridCoords> GetAllFlagCoords() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<ACell*> GetAllBombCells() const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<FGridCoords> GetAllBombCoords() const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<ABomb*> GetAllBombs() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void Init(int W, int L);

	UFUNCTION(BlueprintNativeEvent)
	void HandleCellAdded(ACell* NewCell);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateCell(ACell* CellToUpdate);

private:
	UPROPERTY()
	bool bInitialized;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Length;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Width;

	UPROPERTY()
	TArray<ACell*> Cells;
	UFUNCTION()
	int GetCellIndex1D(int Column, int Row) const;
	UFUNCTION()
	static int Index1DFromIndex2D(int X, int SizeX, int Y, int SizeY);
	UFUNCTION()
	void GetCellIndex2D(int Idx1D, int& Column, int& Row) const;
	UFUNCTION()
	static void Index2DFromIndex1D(int Idx1D, int SizeX, int SizeY, int& X, int& Y);

	UFUNCTION()
	void UpdateMazeSize(int NewWidth, int NewLength);
};
