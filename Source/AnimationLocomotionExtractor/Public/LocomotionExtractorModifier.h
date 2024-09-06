// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "LocomotionExtractorModifier.generated.h"

UCLASS()
class ANIMATIONLOCOMOTIONEXTRACTOR_API ULocomotionExtractorModifier : public UAnimationModifier
{
	GENERATED_BODY()
	
public:

	/** Rate used to sample the animation. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = "1"))
	int32 SampleRate = 30;

	/** Name for the generated curve. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FName CurveName = "Speed";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FName CurveNameX = "SpeedX";	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FName CurveNameY = "SpeedY";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FName CurveNameZ = "SpeedZ";

	virtual void OnApply_Implementation(UAnimSequence* Animation) override;
	virtual void OnRevert_Implementation(UAnimSequence* Animation) override;
};
