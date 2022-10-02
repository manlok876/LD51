// Copyright Denis Maximenko, July 2022

#pragma once

#include "CoreMinimal.h"
#include "GridStructs.generated.h"


USTRUCT(BlueprintType)
struct FGridCoords
{
	GENERATED_BODY()

public:
	FGridCoords();
	FGridCoords(int InColumn, int InRow);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Column;

	bool operator==(const FGridCoords& Point) const;
	bool operator!=(const FGridCoords& Point) const;

	FGridCoords operator+(const FGridCoords& Point) const;
	FGridCoords operator-(const FGridCoords& Point) const;

};
