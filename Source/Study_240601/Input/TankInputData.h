// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UObject/NoExportTypes.h"
#include "TankInputData.generated.h"

/**
 * 
 */
UCLASS()
class STUDY_240601_API UTankInputData : public UObject
{
	GENERATED_BODY()

public:
	UTankInputData();

public:
	UInputMappingContext* mTankContext = nullptr; // 선언 및 초기화

public:
	UInputAction* mMove = nullptr;
	UInputAction* mAttack = nullptr;
	UInputAction* mShield = nullptr;
	UInputAction* mRotation = nullptr;
};
