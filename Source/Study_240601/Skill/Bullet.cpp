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
	//mMovement->bShouldBounce = true; // Bounce Ư��

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Sphere.Sphere'")); // ����ƽ �޽� ������Ʈ�� ���� �޽� ���� ����
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

	mMovement->OnProjectileStop.AddDynamic(this, &ABullet::ProjectileHit); // ��Ƽ ĳ��Ʈ �Լ��̱� ������ ���� ���� �Լ� ��� ����
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::ProjectileHit(const FHitResult& ImpactResult)
{
	// �ε尡 �ȵǾ��ִ� ������Ʈ ���۷����� �� ���
	UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_Impact_Enemy_Fire_Strong.P_Impact_Enemy_Fire_Strong'"));

	// �ε尡 �Ǿ��ִ� ������Ʈ ���۷����� �� ���
	//FindObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_Impact_Enemy_Fire_Strong.P_Impact_Enemy_Fire_Strong'"));

	// SpawnEmitterAttached() : ���� ������ ��ƼŬ�� �����ϰ� �� ��ƼŬ�� �ڽ����� ����� 
	// SpawnEmitterAtLocation() : ���� ������ ��ƼŬ�� ���ͷθ���� ���ϴ� ������ ����
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, ImpactResult.ImpactPoint, FRotator::ZeroRotator , true);

	// ����
	USoundBase* HitSound = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sound/Fire1.Fire1'"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, ImpactResult.ImpactPoint, 0.75f);

	// ������ �Լ�
	// GetActor() : �浹�� ���͸� ���´�
	// TakeDamage() : Actor Ŭ������ �����Լ��� ����� �Լ�. Actor�� ��ӹ��� Ŭ�������� �������Ͽ� ��� ����
	FDamageEvent DmgEvent;
	ImpactResult.GetActor()->TakeDamage(mDamage, DmgEvent, mOwnerController, this);

	Destroy(); // Unreal -> ������Ʈ�� �ƴ� ���͸� �����ؾ��Ѵ�

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ImpactResult.ImpactPoint.ToString());
}

