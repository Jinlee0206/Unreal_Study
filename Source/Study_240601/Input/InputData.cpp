// Fill out your copyright notice in the Description page of Project Settings.


#include "InputData.h"

UDefaultInputData::UDefaultInputData()
{
	// 생성자에서 에셋을 로드하는 방법

	// ConstructorHelpers : 오로지 생성자에서만 사용할 수 있는 구조체
	ConstructorHelpers::FObjectFinder<UInputMappingContext>Context(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Test/Input/IMC_Test.IMC_Test'"));

	// 참조를 성공했을 경우 Context.Object를 미리 만든 mDefaultContext에 할당
	if (Context.Succeeded()) mDefaultContext = Context.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>MoveFB(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move_FB.IA_Move_FB'"));

	if (MoveFB.Succeeded()) mMoveFB = MoveFB.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>MoveLR(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Move_LR.IA_Move_LR'"));

	if (MoveLR.Succeeded()) mMoveLR = MoveLR.Object;

	ConstructorHelpers::FObjectFinder<UInputAction>Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Test/Input/IA_Attack.IA_Attack'"));

	if (Attack.Succeeded()) mAttack = Attack.Object;

}
