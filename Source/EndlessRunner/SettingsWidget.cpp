// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Framerate.h"
#include "SelectionBase.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

namespace
{
	constexpr EFramerate FramerateOptions[] = {
		EFramerate::FPS_30,
		EFramerate::FPS_48,
		EFramerate::FPS_60,
		EFramerate::FPS_120,
		EFramerate::FPS_Uncapped,		
	};

	typedef int32(UGameUserSettings::*GetFunc)() const;
	typedef void(UGameUserSettings::*SetFunc)(int);
	struct FSelectionElement
	{
		USelectionBase* Widget;
		GetFunc GetFunc;
		SetFunc SetFunc;
	};
}

void USettingsWidget::NativeConstruct()
{
	GameUserSettings = UGameUserSettings::GetGameUserSettings();

	InitializeResolutionComboBox();
	InitializeVSync();
	InitializeFramerate();

	const FSelectionElement SelectionElements[] = {
	{ShadingQualitySelection, &UGameUserSettings::GetShadingQuality, &UGameUserSettings::SetShadingQuality},
	{GlobalIlluminationQualitySelection, &UGameUserSettings::GetGlobalIlluminationQuality, &UGameUserSettings::SetGlobalIlluminationQuality},
	{PostProcessingQualitySelection, &UGameUserSettings::GetPostProcessingQuality, &UGameUserSettings::SetPostProcessingQuality},
	{VisualEffectsQualitySelection, &UGameUserSettings::GetVisualEffectQuality, &UGameUserSettings::SetVisualEffectQuality},
	{ShadowQualitySelection, &UGameUserSettings::GetShadowQuality, &UGameUserSettings::SetShadowQuality},		
	};

	for (const auto& [Widget, GetFunc, SetFunc] : SelectionElements)
	{
		const auto CurrentSelection = std::invoke(GetFunc, GameUserSettings);
		Widget->SetCurrentSelection(CurrentSelection);
		Widget->OnSelectionChange.BindLambda([this, SetFunc](int InSelection)
		{
			std::invoke(SetFunc, GameUserSettings, InSelection);
			GameUserSettings->ApplySettings(false);
		});
	}
}

UWidget* USettingsWidget::NativeGetDesiredFocusTarget() const
{
	return ResolutionComboBox;
}

void USettingsWidget::InitializeResolutionComboBox()
{
	Resolutions.Reset();

	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	// Set resolution options
	ResolutionComboBox->ClearOptions();

	for (const auto& Resolution : Resolutions)
	{
		//1920x1080
		const auto ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
	}

	// Find current resolution
	const auto CurrentResolution = GameUserSettings->GetScreenResolution();
	const auto SelectedIndex = Resolutions.IndexOfByPredicate([&CurrentResolution](const FIntPoint& InResolution)
	{
		return InResolution == CurrentResolution;
	});
	check(SelectedIndex >= 0);
	ResolutionComboBox->SetSelectedIndex(SelectedIndex);

	// Listen to changes
	ResolutionComboBox->OnSelectionChanged.Clear();
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &USettingsWidget::OnResolutionChanged);	
}

void USettingsWidget::InitializeVSync()
{
	VSyncCheckbox->SetIsChecked(GameUserSettings->IsVSyncEnabled());
	VSyncCheckbox->OnCheckStateChanged.Clear();
	VSyncCheckbox->OnCheckStateChanged.AddDynamic(this, &USettingsWidget::OnVSyncChanged);
}

void USettingsWidget::InitializeFramerate()
{
	FramerateSelection->Clear();
	int FramerateOptionIndex = 0;
	
	const auto CurrentFramerate = GameUserSettings->GetFrameRateLimit();
	for (const auto& Framerate : FramerateOptions)
	{
		FramerateSelection->AddOption({
			FText::FromString(FFramerateUtils::EnumToString(Framerate))
		});

		if (CurrentFramerate == FFramerateUtils::EnumToValue(Framerate))
		{
			FramerateSelection->SetCurrentSelection(FramerateOptionIndex);
		}

		FramerateOptionIndex++;
	}

	FramerateSelection->OnSelectionChange.BindLambda([this](const int InSelection)
	{
		GameUserSettings->SetFrameRateLimit(FFramerateUtils::EnumToValue(FramerateOptions[InSelection]));
		GameUserSettings->ApplySettings(false);
	});
	
}

void USettingsWidget::OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectInfo)
{
	const auto SelectedResolution = Resolutions[ResolutionComboBox->GetSelectedIndex()];
	GameUserSettings->SetScreenResolution(SelectedResolution);
	GameUserSettings->ApplySettings(false);
}

void USettingsWidget::OnVSyncChanged(bool InIsChecked)
{
	GameUserSettings->SetVSyncEnabled(InIsChecked);
	GameUserSettings->ApplySettings(false);
}
