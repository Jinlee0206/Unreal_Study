// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::ComputeMoveDir(const FVector& ActionValue)
{
	// 생성된 애니메이션 인스턴스(애님인스턴스)를 C++ 코드에서 사용할 수 있어야 한다
	// AnimInstance를 하나 만들어 두고 BeginPlay()에서 ComputeMoveDir()함수를 호출

	mMoveDir = 0.f;
	if (ActionValue.Y > 0.f) {
		if (ActionValue.X > 0.f)
		{
			mMoveDir = 45.f;
		}
		else if (ActionValue.X < 0.f)
		{
			mMoveDir = -45.f;
		}
		else mMoveDir = 0.f;
	}
	else if (ActionValue.Y < 0.f)
	{
		if (ActionValue.X > 0.f)
		{
			mMoveDir = 135.f;
		}
		else if (ActionValue.X < 0.f)
		{
			mMoveDir = -135.f;
		}
		else mMoveDir = 180.f;
	}
	else
	{
		if (ActionValue.X > 0.f) mMoveDir = 90.f;
		else if (ActionValue.X < 0.f) mMoveDir = -90.f;
	}
}

void UPlayerAnimInstance::ComputeAOData(const FVector& ActionValue)
{
	mAOSide += 90.0 * GetWorld()->GetDeltaSeconds() * ActionValue.X;
	mAOLookUp += 90.0 * GetWorld()->GetDeltaSeconds() * ActionValue.Y;

	if (mAOSide < -180.f) mAOSide = -180.f;
	else if (mAOSide > 180.f) mAOSide = 180.f;

	if (mAOLookUp < -90.f) mAOLookUp = -90.f;
	else if (mAOLookUp > 90.f) mAOLookUp = 90.f;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	TObjectPtr<APlayerCharacter> PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UCharacterMovementComponent> CharMovement = PlayerCharacter->GetCharacterMovement(); // 캐릭터 무브먼트는 폰 무브먼트를 상속받음
		//TObjectPtr<UPawnMovementComponent> CharMovement = PlayerCharacter->GetCharacterMovement(); 

		mMoveSpeed = CharMovement->Velocity.Size();

		mIsInAir = CharMovement->IsFalling();

		mAcceleration = CharMovement->GetCurrentAcceleration().Length() > 0.f;
	}
}

void UPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UPlayerAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UPlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}
