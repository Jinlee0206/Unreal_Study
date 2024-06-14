// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	mBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));

	SetRootComponent(mBody);
	mBodyMesh->SetupAttachment(mBody);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Body.Body'"));
	if (BodyMeshAsset.Succeeded()) mBodyMesh->SetStaticMesh(BodyMeshAsset.Object);

	// 박스컴포넌트 사이즈 조정
	mBody->SetBoxExtent(FVector(50.0, 50.0, 50.0));

	mBody->SetCollisionProfileName(TEXT("Item"));
	mBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Block
	//mBody->OnComponentHit

	// Overlap
	mBody->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::ItemBeginOverlap);
	
	mBody->OnComponentEndOverlap.AddDynamic(this, &AItemActor::ItemEndOverlap);
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::ItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s Item Begin Overlap"), *OtherActor->GetName()));
}

void AItemActor::ItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s Item End Overlap"), *OtherActor->GetName()));
}

