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

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> mBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mMesh; // 불렛의 메쉬를 위한 StaticMeshComponent

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mMovement; // 투사체 발사를 위한 ProjectileMovementComponent

	TObjectPtr<AController> mOwnerController; // EventInstigator를 지정하기 위한 컨트롤러 미리 선언

	float mDamage = 10.f; // 총알의 데미지 다르게 설정 가능

public:
	void SetDamage(float Damage)
	{
		mDamage = Damage;
	}

	void SetOwnerController(AController* OwnerController)
	{
		mOwnerController = OwnerController;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 델리게이트에 등혹할 함수는 반드시 UFUNCTION() 지시문을 붙여주어야 한다
	UFUNCTION()
	void ProjectileHit(const FHitResult& ImpactResult); // 부딪히는 대상을 HitResult에 저장

};
