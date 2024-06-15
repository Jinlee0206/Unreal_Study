// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class STUDY_240601_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mBodyMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mHeadMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mBarrelMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> mMuzzle; // ���� ���� �� ������Ʈ(�� ���� ������Ʈ�� ����)

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> mMovement; // �̵��� ���� PlayerPawn Ŭ������ FloatingPawnMovement ������Ʈ �߰�

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> mArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> mCamera;


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
	void OnRotation(const FInputActionValue& InputValue);
};
