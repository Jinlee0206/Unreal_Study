// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerPawn.h"

ADefaultGameMode::ADefaultGameMode()
{
	// 모든 언리얼 클래스들은 클래스이름::StaticClass() 함수를 이용해서 어떤 타입(자신의 클래스 정보)인지를 반환하게 만들어두었다.
	//DefaultPawnClass = APlayerCharacter::StaticClass();

	//DefaultPawnClass = APlayerPawn::StaticClass();
	DefaultPawnClass = nullptr;
}