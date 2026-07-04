// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Stamina.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UVM_Stamina : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UVM_Stamina();

	UFUNCTION(BlueprintPure)
	float GetStamina() const {return Stamina;} 
	
	UFUNCTION(BlueprintCallable)
	void SetStamina(const float &NewHealth);

	UFUNCTION(BlueprintPure, FieldNotify)
	const float GetStaminaPercent() const {return Stamina / MaxStamina;}
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	float Stamina = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	float MaxStamina = 100.0f;
	
};
