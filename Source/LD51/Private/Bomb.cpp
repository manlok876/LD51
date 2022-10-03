// Copyright Denis Maximenko, 2022


#include "Bomb.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxCycles = 6;
	CycleCounter = 0;
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();

}

void ABomb::Explode_Implementation()
{
}

void ABomb::IncrementCycles()
{
	++CycleCounter;

	if (CycleCounter >= MaxCycles)
	{
		Explode();
	}
}

int ABomb::NumCyclesUntilBoom() const
{
	return MaxCycles - CycleCounter;
}
