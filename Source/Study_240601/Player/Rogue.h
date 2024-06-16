// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "Rogue.generated.h"

/**
 * 
 */
UCLASS()
class STUDY_240601_API ARogue : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	ARogue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void PlayAttack();

public:
	virtual void Attack();
};
