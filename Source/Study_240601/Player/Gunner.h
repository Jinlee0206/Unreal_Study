// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "Gunner.generated.h"

UCLASS()
class STUDY_240601_API AGunner : public APawn
{
	GENERATED_BODY()

public:
	AGunner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
