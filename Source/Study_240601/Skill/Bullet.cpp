// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(mBody);
	mMesh->SetupAttachment(mBody);

	mMovement->SetUpdatedComponent(mBody);

	mMovement->InitialSpeed = 1500.f;
	//mMovement->bShouldBounce = true; // Bounce 특성

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Sphere.Sphere'")); // 스태틱 메쉬 컴포넌트로 부터 메쉬 에샛 생성
	if (MeshAsset.Succeeded()) mMesh->SetStaticMesh(MeshAsset.Object);

	//mMesh->SetWorldScale3D(FVector(0.5, 0.5, 0.5));	

	mBody->SetCollisionProfileName(TEXT("PlayerAttack"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mBody->SetSphereRadius(50.f);

	SetLifeSpan(5.f);


}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	mMovement->OnProjectileStop.AddDynamic(this, &ABullet::ProjectileHit); // 멀티 캐스트 함수이기 때문에 여러 개의 함수 등록 가능
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::ProjectileHit(const FHitResult& ImpactResult)
{
	// 로드가 안되어있는 오브젝트 레퍼런싱할 때 사용
	UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_Impact_Enemy_Fire_Strong.P_Impact_Enemy_Fire_Strong'"));

	// 로드가 되어있는 오브젝트 레퍼런싱할 때 사용
	//FindObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_Impact_Enemy_Fire_Strong.P_Impact_Enemy_Fire_Strong'"));

	// SpawnEmitterAttached() : 내가 지정한 파티클을 스폰하고 그 파티클을 자식으로 만들기 
	// SpawnEmitterAtLocation() : 내가 지정한 파티클을 액터로만들고 원하는 지점에 스폰
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, ImpactResult.ImpactPoint, FRotator::ZeroRotator , true);

	// 사운드
	USoundBase* HitSound = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sound/Fire1.Fire1'"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, ImpactResult.ImpactPoint, 0.75f);

	// 데미지 함수
	// GetActor() : 충돌된 액터를 얻어온다
	// TakeDamage() : Actor 클래스에 가상함수로 선언된 함수. Actor를 상속받은 클래스에서 재정의하여 사용 가능
	FDamageEvent DmgEvent;
	ImpactResult.GetActor()->TakeDamage(mDamage, DmgEvent, mOwnerController, this);

	Destroy(); // Unreal -> 컴포넌트가 아닌 액터를 제거해야한다

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ImpactResult.ImpactPoint.ToString());
}

