#pragma once

UENUM(BlueprintType)
enum class EFrameRate: uint8
{
	FPS_30 UMETA(DisplayName = "30 FPS"),
	FPS_48 UMETA(DisplayName = "48 FPS"),
	FPS_60 UMETA(DisplayName = "60 FPS"),
	FPS_120 UMETA(DisplayName = "120 FPS"),
	FPS_Uncapped UMETA(DisplayName = "Uncapped"),
};

class FFRameUtils
{
	static int EnumToValue(const EFrameRate& InFramerate)
	{
		switch (InFramerate)
		{
			case EFrameRate::FPS_30: return 30;
			case EFrameRate::FPS_48: return 48;
			case EFrameRate::FPS_60: return 60;
			case EFrameRate::FPS_120: return 120;
			default: return 0;
		}
	}

	static FString EnumToString(const EFrameRate& InFramerate)
	{
		const auto Value = EnumToValue(InFramerate);
		return Value > 0 ? FString::Printf(TEXT("%d FPS"), Value) : TEXT("Uncapped");
	}
	
};