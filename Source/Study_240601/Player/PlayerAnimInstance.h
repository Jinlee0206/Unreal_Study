// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDY_240601_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mMoveSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsInAir = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mAcceleration = false;

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativePostEvaluateAnimation();
	virtual void NativeUninitializeAnimation();
	virtual void NativeBeginPlay();
};
