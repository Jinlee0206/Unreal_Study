// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class STUDY_240601_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mMesh; // �ҷ��� �޽��� ���� StaticMeshComponent

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mMovement; // ����ü �߻縦 ���� ProjectileMovementComponent

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// �K������Ʈ�� ����� �Լ��� �ݵ�� UFUNCTION()�� �ٿ��־�� �Ѵ�
	UFUNCTION()
	void ProjectileHit(const FHitResult& ImpactResult); // �ε����� ����� HitResult�� ����

};
