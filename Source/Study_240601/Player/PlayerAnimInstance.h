// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 뼈대가 되는 AnimInstance AnimationBP를 객체로 만들어내서 사용할 클래스
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
	float mMoveDir = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mAOSide= 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mAOLookUp = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsInAir = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mAcceleration = false;

public:
	void ComputeMoveDir(const FVector& ActionValue); // 액션 밸류를 참조로 받아 최종 방향을 만들어 줌
	void ComputeAOData(const FVector& ActionValue);

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativePostEvaluateAnimation();
	virtual void NativeUninitializeAnimation();
	virtual void NativeBeginPlay();
};
