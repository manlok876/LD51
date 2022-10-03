// Copyright Denis Maximenko, 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class LD51_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:
	ABomb();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCycles;

	UFUNCTION(BlueprintCallable)
	void IncrementCycles();

	UFUNCTION(BlueprintPure)
	int NumCyclesUntilBoom() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CycleCounter;
};
