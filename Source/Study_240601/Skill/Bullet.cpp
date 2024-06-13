// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(mMesh);

	mMovement->SetUpdatedComponent(mMesh);

	mMovement->InitialSpeed = 1500.f;
	//mMovement->bShouldBounce = true; // Bounce Ư��

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Sphere.Sphere'")); // ����ƽ �޽� ������Ʈ�� ���� �޽� ���� ����
	if (MeshAsset.Succeeded()) mMesh->SetStaticMesh(MeshAsset.Object);

	mMesh->SetWorldScale3D(FVector(0.5, 0.5, 0.5));	

	SetLifeSpan(5.f);


}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	mMovement->OnProjectileStop.AddDynamic(this, &ABullet::ProjectileHit); // ��Ƽ ĳ��Ʈ �Լ��̱� ������ ���� ���� �Լ� ��� ����
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::ProjectileHit(const FHitResult& ImpactResult)
{
	Destroy(); // Unreal -> ������Ʈ�� �ƴ� ���͸� �����ؾ��Ѵ�

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ImpactResult.ImpactPoint.ToString());
}

