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

	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetStamina() const { return Stamina; }
	
	UFUNCTION(BlueprintCallable)
	void SetStamina(const int32 &NewStamina);	

	UFUNCTION(BlueprintCallable)
	void ConsumeStamina(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void RestoreStamina(int32 Amount);
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	int32 Stamina = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 MaxStamina = 5;			
};
