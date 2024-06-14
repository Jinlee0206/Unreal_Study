// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Pawn.h"
#include "AITank.generated.h"

UCLASS()
class STUDY_240601_API AAITank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAITank();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
};
