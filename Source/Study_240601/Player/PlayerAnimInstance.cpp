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

void UPlayerAnimInstance::SetAOData(float Pitch, float Yaw)
{
	mAOSide = Yaw;
	mAOLookUp = Pitch;
}

void UPlayerAnimInstance::OnJump()
{
	mJump = true;
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

		if (!mIsInAir) mJump = false;

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
	
	OnMontageEnded.AddDynamic(this, &UPlayerAnimInstance::MontageEnd);
}

void UPlayerAnimInstance::PlayAttack()
{
	// 몽타쥬 애니메이션 유효 검사
	if (mAttackMontage.IsEmpty()) return;

	// 첫번째 공격 시도 상태일 때, mAttackState가 false인 경우만 실행
	if (!mAttackState)
	{
		// 몽타쥬 재생 중인지 판단
		if (!Montage_IsPlaying(mAttackMontage[0]))
		{
			Montage_SetPosition(mAttackMontage[0], 0.f);
			Montage_Play(mAttackMontage[0]);
			Montage_JumpToSection(mAttackSectionName[mAttackSectionIndex]);
		}
	}
	else
	{
		mAttackCombo = true;
	}
	mAttackState = true;
}

void UPlayerAnimInstance::AnimNotify_Combo()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Combo"));

	if (mAttackCombo)
	{
		mAttackSectionIndex = (mAttackSectionIndex + 1) % mAttackSectionName.Num(); // 인덱스 OutOfRange 방지
		Montage_Play(mAttackMontage[0]);
		Montage_JumpToSection(mAttackSectionName[mAttackSectionIndex]);
		mAttackCombo = false;

		//FOnMontageEnded EndDelegate;
		//EndDelegate.BindUObject(this, &UPlayerAnimInstance::MontageEnd);
		//Montage_SetEndDelegate(EndDelegate, mAttackMontage[0]);
	}
	
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter)) PlayerCharacter->Attack();

}

void UPlayerAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (mAttackMontage[0] == Montage)
	{
		// true인 경우 콤보상태 false인 경우 자연 종료
		if (!bInterrupted)
		{
			mAttackState = false;
			mAttackCombo = false;
			mAttackSectionIndex = 0;

			TObjectPtr<APlayerCharacter> PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

			if (IsValid(PlayerCharacter)) PlayerCharacter->SetMoveEnable(true);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Interrupted false"));
		}
		else GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Interrupted true"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("MontageEnd"));
}
