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
	// TObjectPtr�� ��ü�� �޸� �ּҸ� ������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> mArm; // �𸮾� UObject�� ������ ����

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> mCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> mRotation; // �� ���ӿ�����Ʈ (Rotation ������)

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> mRotationMovement; // RotatingMovementComponent

	// TSubclassOf�� UClass(Ŭ���� Ÿ��) ������ �����Ѵ�
	TSubclassOf<AActor> mBulletClass;

	// ���� �ð� (��Ÿ�Ӹ��� �ʱ�ȭ)
	float mShieldTime = 0.f;
	// ��ų ��Ÿ��
	float mShieldCoolDown = 6.f;
	// ��ų ��밡�� ���� �Ҹ� ��
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
