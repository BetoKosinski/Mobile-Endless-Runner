// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSpawner.h"

#include "BaseLevel.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelSpawner::ALevelSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALevelSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnLevel(true);	
	
}

// Called every frame
void ALevelSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSpawner::SpawnLevel(bool IsFirst)
{
	SpawnLocation = FVector(0.f, 1000.f, 0.f);
	SpawnRotation = FRotator(0.f, 90.f, 0.f);

	if(!IsFirst)
	{
		ABaseLevel* LastLevel = LevelList.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}

	RandomLevel = FMath::RandRange(1, 5);
	ABaseLevel* NewLevel = nullptr;

	if (LevelClasses.Num() == 0)
	{
		return;
	} 

	int32 RandomIndex = FMath::RandRange(0, LevelClasses.Num() - 1);
	NewLevel = GetWorld()->SpawnActor<ABaseLevel>(LevelClasses[RandomIndex], SpawnLocation, SpawnRotation);

	if (!LevelClasses[RandomIndex]) 
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelSpawner: Invalid LevelClass Index %d"), RandomIndex);
		return;
	}

	if(NewLevel)
	{
		if(NewLevel->GetTrigger())
		{
			NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ALevelSpawner::OnOverlapBegin);
		}
	}

	LevelList.Add(NewLevel);

	if (LevelList.Num() > 5)
	{
		ABaseLevel* OldLevel = LevelList[0];
		LevelList.RemoveAt(0);
		
		if (OldLevel)
		{
			OldLevel->Destroy();
		}
	}
	
}

void ALevelSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnLevel(false);	
}

