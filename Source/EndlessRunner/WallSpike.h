// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spikes.h"
#include "WallSpike.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AWallSpike : public ASpikes
{
	GENERATED_BODY()

public:
	AWallSpike();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ScrollSpeed = 350.0f;
	
};
