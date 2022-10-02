// Fill out your copyright notice in the Description page of Project Settings.


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
