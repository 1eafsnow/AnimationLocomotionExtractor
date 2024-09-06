// Fill out your copyright notice in the Description page of Project Settings.


#include "LocomotionExtractorModifier.h"
#include "Animation/AnimSequence.h"
#include "AnimationBlueprintLibrary.h"
#include "EngineLogs.h"

// TODO: This logic works decently for simple clips but it should be reworked to be more robust:
//  * It could detect pivot points by change in direction.
//  * It should also account for clips that have multiple stop/pivot points.
//  * It should handle distance traveled for the ends of looping animations.
void ULocomotionExtractorModifier::OnApply_Implementation(UAnimSequence* Animation)
{
	if (Animation == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("DistanceCurveModifier failed. Reason: Invalid Animation"));
		return;
	}

	if (!Animation->HasRootMotion())
	{
		UE_LOG(LogAnimation, Error, TEXT("DistanceCurveModifier failed. Reason: Root motion is disabled on the animation (%s)"), *GetNameSafe(Animation));
		return;
	}

	const bool bMetaDataCurve = false;
	UAnimationBlueprintLibrary::AddCurve(Animation, CurveName, ERawCurveTrackTypes::RCT_Float, bMetaDataCurve);
	UAnimationBlueprintLibrary::AddCurve(Animation, CurveNameX, ERawCurveTrackTypes::RCT_Float, bMetaDataCurve);
	UAnimationBlueprintLibrary::AddCurve(Animation, CurveNameY, ERawCurveTrackTypes::RCT_Float, bMetaDataCurve);
	UAnimationBlueprintLibrary::AddCurve(Animation, CurveNameZ, ERawCurveTrackTypes::RCT_Float, bMetaDataCurve);

	const float AnimLength = Animation->GetPlayLength();
	float SampleInterval;
	int32 NumSteps;

	SampleInterval = 1.f / SampleRate;
	NumSteps = FMath::CeilToInt(AnimLength / SampleInterval);
	float Time = 0.0f;
	for (int32 Step = 0; Step <= NumSteps && Time < AnimLength; ++Step)
	{
		float EndTime = FMath::Min((Step + 1) * SampleInterval, AnimLength);

		const FVector RootMotionTranslation = Animation->ExtractRootMotionFromRange(Time, EndTime).GetTranslation() / SampleInterval;
		UAnimationBlueprintLibrary::AddFloatCurveKey(Animation, CurveName, Time, RootMotionTranslation.Length());
		UAnimationBlueprintLibrary::AddFloatCurveKey(Animation, CurveNameX, Time, RootMotionTranslation.X);
		UAnimationBlueprintLibrary::AddFloatCurveKey(Animation, CurveNameY, Time, RootMotionTranslation.Y);
		UAnimationBlueprintLibrary::AddFloatCurveKey(Animation, CurveNameZ, Time, RootMotionTranslation.Z);

		Time = EndTime;
	}
}

void ULocomotionExtractorModifier::OnRevert_Implementation(UAnimSequence* Animation)
{
	//const bool bRemoveNameFromSkeleton = false;
	//UAnimationBlueprintLibrary::RemoveCurve(Animation, CurveName, bRemoveNameFromSkeleton);
}