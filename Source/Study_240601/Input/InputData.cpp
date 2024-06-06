// Fill out your copyright notice in the Description page of Project Settings.


#include "InputData.h"

UDefaultInputData::UDefaultInputData()
{
	// �����ڿ��� ������ �ε��ϴ� ���

	// ConstructorHelpers : ������ �����ڿ����� ����� �� �ִ� ����ü
	ConstructorHelpers::FObjectFinder<UInputMappingContext>Context(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Test/Input/IMC_Test.IMC_Test'"));

	// ������ �������� ��� Context.Object�� �̸� ���� mDefaultContext�� �Ҵ�
	if (Context.Succeeded()) mDefaultContext = Context.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>MoveFB(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move_FB.IA_Move_FB'"));

	if (MoveFB.Succeeded()) mMoveFB = MoveFB.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>MoveLR(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move_LR.IA_Move_LR'"));

	if (MoveLR.Succeeded()) mMoveLR = MoveLR.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Attack.IA_Attack'"));

	if (Attack.Succeeded()) mAttack = Attack.Object;

}
