// Fill out your copyright notice in the Description page of Project Settings.


#include "VM_Stamina.h"

UVM_Stamina::UVM_Stamina()
{
	Stamina = MaxStamina;
}

void UVM_Stamina::SetStamina(const int32& NewStamina)
{
	const int32 Clamped = FMath::Clamp(NewStamina, 0, MaxStamina);
	if (UE_MVVM_SET_PROPERTY_VALUE(Stamina, Clamped))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStamina);
	}
}

void UVM_Stamina::ConsumeStamina(int32 Amount)
{
	if (Stamina > 0)
	{
		SetStamina(Stamina - Amount);
	}	
}

void UVM_Stamina::RestoreStamina(int32 Amount)
{
	if (Stamina < MaxStamina)
	{
		SetStamina(Stamina + Amount);
	}
}




