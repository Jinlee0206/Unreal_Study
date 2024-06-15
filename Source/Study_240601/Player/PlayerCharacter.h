// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class STUDY_240601_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// TObjectPtr는 객체의 메모리 주소를 저장함
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> mArm; // 언리얼 UObject용 포인터 선언

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> mCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> mRotation; // 빈 게임오브젝트 (Rotation 기준점)

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> mRotationMovement; // RotatingMovementComponent

	TObjectPtr<class UPlayerAnimInstance> mAnimInst;

	// TSubclassOf는 UClass(클래스 타입) 정보를 저장한다
	TSubclassOf<AActor> mBulletClass;

	// 누적 시간(쿨타임마다 초기화)
	float mShieldTime = 0.f;
	// 스킬 쿨타임
	float mShieldCoolDown = 6.f;
	// 스킬 사용가능 여부
	bool mShieldEnable = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnMove(const FInputActionValue& InputValue);
	void OnAttack(const FInputActionValue& InputValue);
	void OnShield(const FInputActionValue& InputValue);
	void OnRotation(const FInputActionValue& InputValue);
	void OnCameraZoom(const FInputActionValue& InputValue);
};
