// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
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
