// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

class ABomb;

UENUM(BlueprintType)
enum class ECellState : uint8
{
	ECS_Open	UMETA(DisplayName = "Open"),
	ECS_Closed	UMETA(DisplayName = "Closed"),
	ECS_Flag	UMETA(DisplayName = "Flagged"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCellStateChangedSignature, ACell*, Cell, ECellState, NewState);

UCLASS()
class LD51_API ACell : public AActor
{
	GENERATED_BODY()

public:
	ACell();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCellSize(const FVector2D& NewSize);

	UFUNCTION(BlueprintPure)
	bool IsFlagged() const;
	UFUNCTION(BlueprintPure)
	bool IsOpen() const;
	UFUNCTION(BlueprintPure)
	bool IsClosed() const;

	ECellState GetCellState() const;
	UFUNCTION(BlueprintCallable)
	void SetCellState(ECellState NewState);
	UFUNCTION(BlueprintImplementableEvent)
	void OnCellStateChanged(ECellState NewState);
	UPROPERTY(BlueprintAssignable)
	FCellStateChangedSignature CellStateChangedDispatcher;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlaceFlag();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveFlag();

	UFUNCTION(BlueprintCallable)
	void OpenCell();
	UFUNCTION(BlueprintCallable)
	void CoverCell();

	UFUNCTION(BlueprintPure)
	bool HasBomb() const;

	ABomb* GetBomb() const;
	UFUNCTION(BlueprintCallable)
	void PlantBomb(ABomb* NewBomb);
	UFUNCTION(BlueprintCallable)
	void ClearBomb(bool bDestroy = false);

	UFUNCTION(BlueprintPure)
	bool CanActivate() const;
	UFUNCTION(BlueprintCallable)
	void Activate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BombsAround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ACell*> NeighborsCache;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECellState State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABomb* Bomb;
};
