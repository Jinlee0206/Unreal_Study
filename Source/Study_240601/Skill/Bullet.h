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
	TObjectPtr<UStaticMeshComponent> mMesh; // 불렛의 메쉬를 위한 StaticMeshComponent

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mMovement; // 투사체 발사를 위한 ProjectileMovementComponent

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 덷리게이트에 등록할 함수는 반드시 UFUNCTION()을 붙여주어야 한다
	UFUNCTION()
	void ProjectileHit(const FHitResult& ImpactResult); // 부딪히는 대상을 HitResult에 저장

};
