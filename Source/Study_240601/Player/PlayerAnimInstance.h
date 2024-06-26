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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> mAttackMontage; // 동적배열 런타임 중 접근가능

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> mAttackSectionName; // 공격 섹션을 저장하기 위한 배열

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool mJump = false; // 점프 상태인지 확인하는 변수

	int32 mAttackSectionIndex = 0; // 콤보 어택 시 섹션 인덱스를 미리 저장
	bool mAttackState = false; // 공격 상태가 아님
	bool mAttackCombo = false; // 콤보 공격 상태가 아님

public:
	void ComputeMoveDir(const FVector& ActionValue); // 액션 밸류를 참조로 받아 최종 방향을 만들어 줌
	void ComputeAOData(const FVector& ActionValue);
	void SetAOData(float Pitch, float Yaw);
	void OnJump();

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativePostEvaluateAnimation();
	virtual void NativeUninitializeAnimation();
	virtual void NativeBeginPlay();

	void PlayAttack();

	// Notify 함수를 만들 때, AnimNotify_함수이름() 꼴로 생성
	UFUNCTION()
	void AnimNotify_Combo();

	UFUNCTION()
	void AnimNotify_Attack();

	// 언리얼 런타임에 함수 찾기 위해 선언
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
