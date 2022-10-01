// Copyright Denis Maximenko, July 2022


#include "GridStructs.h"


FGridCoords::FGridCoords()
{
	Row = 0;
	Column = 0;
}

FGridCoords::FGridCoords(int InColumn, int InRow)
{
	Row = InRow;
	Column = InColumn;
}

bool FGridCoords::operator==(const FGridCoords& Point) const
{
	return this->Row == Point.Row && this->Column == Point.Column;
}

bool FGridCoords::operator!=(const FGridCoords& Point) const
{
	return this->Row != Point.Row || this->Column != Point.Column;
}

FGridCoords FGridCoords::operator+(const FGridCoords& Point) const
{
	return FGridCoords(this->Column + Point.Column, this->Row + Point.Row);
}

FGridCoords FGridCoords::operator-(const FGridCoords& Point) const
{
	return FGridCoords(this->Column - Point.Column, this->Row - Point.Row);
}
