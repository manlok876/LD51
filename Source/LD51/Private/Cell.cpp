// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "Bomb.h"

ACell::ACell()
{
	PrimaryActorTick.bCanEverTick = true;

	State = ECellState::ECS_Closed;
	Bomb = nullptr;
	BombsAround = 0;
}

void ACell::BeginPlay()
{
	Super::BeginPlay();

}

bool ACell::IsFlagged() const
{
	return GetCellState() == ECellState::ECS_Flag;
}

bool ACell::IsOpen() const
{
	return GetCellState() == ECellState::ECS_Open;
}

bool ACell::IsClosed() const
{
	return GetCellState() == ECellState::ECS_Closed;
}

ECellState ACell::GetCellState() const
{
	return State;
}

void ACell::SetCellState(ECellState NewState)
{
	if (State == NewState)
	{
		return;
	}

	State = NewState;

	OnCellStateChanged(NewState);
	if (CellStateChangedDispatcher.IsBound())
	{
		CellStateChangedDispatcher.Broadcast(this, NewState);
	}
}

void ACell::PlaceFlag_Implementation()
{
	if (IsClosed() && !IsFlagged())
	{
		SetCellState(ECellState::ECS_Flag);
	}
}

void ACell::RemoveFlag_Implementation()
{
	if (IsFlagged())
	{
		SetCellState(ECellState::ECS_Closed);
	}
}

void ACell::OpenCell()
{
	if (IsClosed())
	{
		SetCellState(ECellState::ECS_Open);
	}
}

void ACell::CoverCell()
{
	if (IsOpen())
	{
		SetCellState(ECellState::ECS_Closed);
	}
}

bool ACell::HasBomb() const
{
	return IsValid(GetBomb());
}

ABomb* ACell::GetBomb() const
{
	return Bomb;
}

void ACell::PlantBomb(ABomb* NewBomb)
{
	if (HasBomb())
	{
		// ???
	}

	Bomb = NewBomb;
}

void ACell::ClearBomb(bool bDestroy)
{
	if (!HasBomb())
	{
		return;
	}

	if (bDestroy)
	{
		Bomb->Destroy();
	}

	Bomb = nullptr;
}

bool ACell::CanActivate() const
{
	return IsClosed() && !IsFlagged();
}

void ACell::Activate()
{
	if (!CanActivate())
	{
		return;
	}

	if (IsClosed())
	{
		OpenCell();
	}

	ABomb* CurBomb = GetBomb();
	if (IsValid(CurBomb))
	{
		CurBomb->Explode();
	}
}
