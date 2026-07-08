// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionBase.h"
#include "CommonTextBlock.h"
#include "Logging/StructuredLog.h"
#include "UObject/FastReferenceCollector.h"

USelectionBase::USelectionBase()
{
	CurrentSelection = 0;
	SetIsFocusable(true);
	SetVisibilityInternal(ESlateVisibility::Visible);
}

void USelectionBase::NativeConstruct()
{
	if (Options.Num() == 0)
	{
		UE_LOGFMT(LogTemp, Log, "USelectionBase: No options were provided.");
		return;
	}

	UpdateCurrentSelection();

	FCustomWidgetNavigationDelegate NavigationDelegate;
	NavigationDelegate.BindDynamic(this, &USelectionBase::OnNavigation);
	SetNavigationRuleCustom(EUINavigation::Left, NavigationDelegate);
	SetNavigationRuleCustom(EUINavigation::Right, NavigationDelegate);
}

void USelectionBase::Clear()
{
	Options.Reset();
}

void USelectionBase::AddOption(const FSelectionOption& InOption)
{
	Options.Add(InOption);
	UpdateCurrentSelection();
}

void USelectionBase::SetCurrentSelection(int InIndex)
{
	check(InIndex >= 0 && InIndex < Options.Num());
	CurrentSelection = InIndex;
	UpdateCurrentSelection();
}

UWidget* USelectionBase::OnNavigation(EUINavigation InNavigation)
{
	check(InNavigation == EUINavigation::Left || InNavigation == EUINavigation::Right);

	const auto Direction = InNavigation == EUINavigation::Left ? -1 : 1;

	CurrentSelection += Direction;

	if (CurrentSelection < 0)
	{
		CurrentSelection = Options.Num() - 1;
	}
	else if (CurrentSelection >= Options.Num())
	{
		CurrentSelection = 0;
	}

	UpdateCurrentSelection();

	OnSelectionChange.ExecuteIfBound(CurrentSelection);

	return this;	
}

void USelectionBase::UpdateCurrentSelection()
{
	Label->SetText(Options[CurrentSelection].Label);
}

void USelectionBase::SelectPrevious()
{
	OnNavigation(EUINavigation::Left);
}

void USelectionBase::SelectNext()
{
	OnNavigation(EUINavigation::Right);
}