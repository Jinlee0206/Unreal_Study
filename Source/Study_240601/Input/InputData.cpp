// Fill out your copyright notice in the Description page of Project Settings.


#include "InputData.h"

UDefaultInputData::UDefaultInputData()
{
	// �����ڿ��� ������ �ε��ϴ� ���
	// �̸� ������ �ε��س��� �����ؼ� ����ϴ� ���

	// ConstructorHelpers : ������ �����ڿ����� ����� �� �ִ� ����ü
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>Context(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Test/Input/IMC_Test.IMC_Test'"));

	// ������ �������� ��� Context.Object�� �̸� ���� mDefaultContext�� �Ҵ�
	if (Context.Succeeded()) mDefaultContext = Context.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>Move(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move.IA_Move'"));

	if (Move.Succeeded()) mMove = Move.Object;

	/*
	ConstructorHelpers::FObjectFinder<UInputAction>MoveFB(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move_FB.IA_Move_FB'"));

	if (MoveFB.Succeeded()) mMoveFB = MoveFB.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>MoveLR(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move_LR.IA_Move_LR'"));

	if (MoveLR.Succeeded()) mMoveLR = MoveLR.Object;
	*/

	static ConstructorHelpers::FObjectFinder<UInputAction>Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Attack.IA_Attack'"));

	if (Attack.Succeeded()) mAttack = Attack.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction>Shield(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Shield.IA_Shield'"));

	if (Shield.Succeeded()) mShield = Shield.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>Rotation(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Rotation.IA_Rotation'"));

	if (Rotation.Succeeded()) mRotation = Rotation.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>CameraZoom(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Rotation.IA_CameraZoom'"));

	if (CameraZoom.Succeeded()) mCameraZoom = CameraZoom.Object;
}
