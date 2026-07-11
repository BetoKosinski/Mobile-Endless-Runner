// Fill out your copyright notice in the Description page of Project Settings.


#include "VM_Stamina.h"
#include "TimerManager.h"
#include "Engine/World.h"

UVM_Stamina::UVM_Stamina()
{
	Stamina = MaxStamina;
}

void UVM_Stamina::SetStamina(const float &NewStamina)
{
	const float Clamped = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
	if (UE_MVVM_SET_PROPERTY_VALUE(Stamina, Clamped))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaminaPercent);
	}
}

void UVM_Stamina::ConsumeStamina(float Amount)
{
	SetStamina(Stamina - Amount);

	StopRegeneration();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CooldownTimerHandle);
		World->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			this,
			&UVM_Stamina::StartRegeneration,
			RegenCooldown,
			false
			);
	}	
}

void UVM_Stamina::StartRegeneration()
{
	if (UWorld* World = GetWorld())
	{		
		World->GetTimerManager().SetTimer(
			RegenTimerHandle,
			this,
			&UVM_Stamina::RegenerationTick,
			RegenTickInterval,
			true
			);			
	}
}

void UVM_Stamina::StopRegeneration()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
}

void UVM_Stamina::RegenerationTick()
{
	if (Stamina >= MaxStamina)
	{
		SetStamina(MaxStamina);
		StopRegeneration();
		return;
	}

	SetStamina(Stamina + RegenRate * RegenTickInterval);
}


