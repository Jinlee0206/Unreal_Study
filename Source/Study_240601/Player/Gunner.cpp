#include "Gunner.h"
#include "PlayerAnimInstance.h"

AGunner::AGunner()
{
	PrimaryActorTick.bCanEverTick = true;


	//메쉬
	// /Script/Engine.SkeletalMesh'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Skins/Tier2/ShadowOps/Meshes/TwinBlast_ShadowOps.TwinBlast_ShadowOps'
}

void AGunner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

