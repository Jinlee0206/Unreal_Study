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
	TObjectPtr<USceneComponent> mMuzzle; // 공격 지점 씬 컴포넌트(빈 게임 오브젝트의 개념)

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> mMovement; // 이동을 위해 PlayerPawn 클래스에 FloatingPawnMovement 컴포넌트 추가

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
};
