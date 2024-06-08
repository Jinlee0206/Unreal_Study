// Fill out your copyright notice in the Description page of Project Settings.


#include "TankInputData.h"

UTankInputData::UTankInputData()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>Context(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Test/Input/IMC_Tank.IMC_Tank'"));

	if (Context.Succeeded()) mTankContext = Context.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>Move(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move.IA_Move'"));

	if (Move.Succeeded()) mMove = Move.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Attack.IA_Attack'"));

	if (Attack.Succeeded()) mAttack = Attack.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>Rotation(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Rotation.IA_Rotation'"));

	if (Rotation.Succeeded()) mRotation = Rotation.Object;
}
