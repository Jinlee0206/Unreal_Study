// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerPawn.h"

ADefaultGameMode::ADefaultGameMode()
{
	// ��� �𸮾� Ŭ�������� Ŭ�����̸�::StaticClass() �Լ��� �̿��ؼ� � Ÿ��(�ڽ��� Ŭ���� ����)������ ��ȯ�ϰ� �����ξ���.
	//DefaultPawnClass = APlayerCharacter::StaticClass();

	//DefaultPawnClass = APlayerPawn::StaticClass();
	DefaultPawnClass = nullptr;
}