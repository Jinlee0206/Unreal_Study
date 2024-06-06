// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UObject/NoExportTypes.h"
#include "InputData.generated.h"

/**
 * 
 */
UCLASS()
class STUDY_240601_API UDefaultInputData : public UObject
{
	GENERATED_BODY()
	
public :
	UDefaultInputData();

public:
	UInputMappingContext* mDefaultContext = nullptr; // 선언 및 초기화

public:
	UInputAction* mMoveFB = nullptr;
	UInputAction* mMoveLR = nullptr;
	UInputAction* mAttack = nullptr;


};
