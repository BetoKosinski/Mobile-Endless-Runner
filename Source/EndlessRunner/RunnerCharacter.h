// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VM_Stamina.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* SideViewCamera;

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	TObjectPtr<UVM_Stamina> StaminaViewModel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StaminaWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FVector2D TouchStartLocation;
	FVector LastSwipeWorldPos;
	float SwipeMinDistance = 50.0f;
	float SwipeMaxDistance = 200.0f;
	float SwipeImpulseStrength = 5.0f;
	int32 ActionStaminaCost = 1;	

	UFUNCTION()
	void OnTouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION()
	void OnTouchEnded(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION()
	void OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void MoveRight(float Value);
	virtual void Jump() override;

public:

	class UCameraComponent* GetSideViewCameraComponent() const
	{
		return SideViewCamera;
	}

	void RespawnCharacter();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	float zPosition;
	FVector tempPos = FVector();
	
	bool CanMove;

	TObjectPtr<UUserWidget> StaminaWidgetInstance;

};
