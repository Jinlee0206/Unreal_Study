// Fill out your copyright notice in the Description page of Project Settings.


#include "Rogue.h"
#include "PlayerAnimInstance.h" // UAnimMontage 사용해야 함

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

void ARogue::PlayAttack()
{
	mAnimInst->PlayAttack();
}

void ARogue::Attack()
{
	FHitResult result;
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50.f;
	FVector End = Start + GetActorForwardVector() * 200.f;
	
	FCollisionQueryParams param(NAME_None, false, this);
	bool Collision = GetWorld()->SweepSingleByProfile(result, Start, End, FQuat::Identity, TEXT("PlayerAttack"), FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = Collision ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (Start + End) / 2.f, 100.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 2.f);

#endif

	if (Collision)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Collision"));
		UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Melee_SucessfulImpact_Rogue.P_Melee_SucessfulImpact_Rogue'"));
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, result.ImpactPoint, result.ImpactNormal.Rotation());

		FDamageEvent DmgEvent;
		result.GetActor()->TakeDamage(10.f, DmgEvent, GetController(), this);
	}
}

