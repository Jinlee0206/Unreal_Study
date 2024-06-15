// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerPawn.h"
#include "../Player/Rogue.h"

ADefaultGameMode::ADefaultGameMode()
{
	//모든 언리얼 클래스들은 클래스이름::StaticClass() 함수를 이용해서 어떤 타입(자신의 클래스 정보)인지를 반환할 수 있다
	//DefaultPawnClass = APlayerCharacter::StaticClass();

	//DefaultPawnClass = APlayerPawn::StaticClass();
	DefaultPawnClass = nullptr; // DefaultPawn 없음으로 처리
	//DefaultPawnClass = ARogue::StaticClass();
}