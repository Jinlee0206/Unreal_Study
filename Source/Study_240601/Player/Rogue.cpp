// Fill out your copyright notice in the Description page of Project Settings.


#include "Rogue.h"

ARogue::ARogue()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKallari/Characters/Heroes/Kallari/Skins/Rogue/Meshes/Kallari_Rogue.Kallari_Rogue'"));

	if (MeshAsset.Succeeded()) GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/ABP_Rogue.ABP_Rogue_C'"));

	if (AnimClass.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	GetCapsuleComponent()->SetCapsuleHalfHeight(92.f);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -92.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

}

void ARogue::BeginPlay()
{
	Super::BeginPlay();
}

void ARogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

