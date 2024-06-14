// Fill out your copyright notice in the Description page of Project Settings.


#include "AITank.h"

// Sets default values
AAITank::AAITank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	mBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	SetRootComponent(mBody);
	mBodyMesh->SetupAttachment(mBody);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Body.Body'"));
	if (BodyMeshAsset.Succeeded()) mBodyMesh->SetStaticMesh(BodyMeshAsset.Object);

	mHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	mHeadMesh->SetupAttachment(mBodyMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> HeadMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Head.Head'"));
	if (HeadMeshAsset.Succeeded())
	{
		mHeadMesh->SetStaticMesh(HeadMeshAsset.Object);
	}

	mBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	mBarrelMesh->SetupAttachment(mHeadMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Barrel.Barrel'"));
	if (BarrelMeshAsset.Succeeded())
	{
		mBarrelMesh->SetStaticMesh(BarrelMeshAsset.Object);
	}

	mMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	mMuzzle->SetupAttachment(mBarrelMesh);

	// 메쉬 생성
	mBodyMesh->SetWorldScale3D(FVector(2.0, 2.0, 1.0));
	mHeadMesh->SetRelativeScale3D(FVector(0.5, 0.5, 1.0));
	mHeadMesh->SetRelativeLocation(FVector(0.0, 0.0, 100));
	mBarrelMesh->SetRelativeScale3D(FVector(2.0, 0.25, 0.25));
	mBarrelMesh->SetRelativeLocation(FVector(50, 0, 0));
	mMuzzle->SetRelativeLocation(FVector(115.0, 0, 0));

	// FloatingPawnComponent 추가 (움직임 가능하게)
	mMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(mBodyMesh);

	// 회전
	bUseControllerRotationYaw = true; // true 일때, 폰의 Yaw가 컨트롤러 Yaw 로테이션과 매칭된다 

	// 박스컴포넌트 사이즈 조정
	mBody->SetBoxExtent(FVector(100.0, 100.0, 50.0));

	// 박스컴포넌트는 Player CollisionProfile로 지정하고 나머지 메쉬는 충돌처리 안되게 변경
	mBody->SetCollisionProfileName(TEXT("EnemyTank"));
	mBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mHeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mBarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAITank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAITank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AAITank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dmg : %.5f"), DamageAmount));

	return 0.0f;
}
