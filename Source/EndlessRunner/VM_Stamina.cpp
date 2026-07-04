// Fill out your copyright notice in the Description page of Project Settings.


#include "VM_Stamina.h"

UVM_Stamina::UVM_Stamina()
{
	Stamina = MaxStamina;
}

void UVM_Stamina::SetStamina(const float &NewHealth)
{
	if(UE_MVVM_SET_PROPERTY_VALUE(Stamina, NewHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaminaPercent);
	}
}


