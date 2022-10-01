// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	virtual void BeginPlay() override;
};
