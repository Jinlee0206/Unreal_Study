// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Input/InputData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CreateDefaultSubobject 함수는 생성자에서만 사용한다
	// 템플릿에 지정된 타입의 객체 하나를 생성하고 그 메모리 주소를 반환해준다.
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mArm->SetupAttachment(RootComponent); // SpringArm은 RootComponent의 자식으로 붙여주고

	mCamera->SetupAttachment(mArm); // Camera는 SpringArm의 자식으로 붙여준다

	mArm->TargetArmLength = 500.f;

	mArm->SetRelativeLocation(FVector(0.0, 0.0, 70.0));
	mArm->SetRelativeRotation(FRotator(-10.0, 0.0, 0.0));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// GetController로 컨트롤러를 찾고 PlayerController로 형 변환

	// PlayerController로 부터 LocalPlayer를 얻어옴
	// 미리 PlayerController를 얻어 두고, nullptr이 아니면(찾음) LocalPlayer를 얻어옴
	APlayerController* PlayerController = GetController<APlayerController>();
	if (nullptr != PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		// LocalPlayer를 이용해서 EnhancedInputLocalPlayerSubsystem을 얻어옴
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		// UDefaultInputData의 CDO를 꺼내온다
		const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();

		Subsystem->AddMappingContext(InputData->mDefaultContext, 0);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast : 언리얼 UObject 객체들의 형변환 함수
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// UDefaultInputData의 CDO를 꺼내온다
	const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();

	// 원하는 InputAction이 동작할 때 호출될 함수의 함수 포인터를 지정
	EnhancedInput->BindAction(InputData->mMoveFB, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMoveFB);
}

void APlayerCharacter::OnMoveFB(const FInputActionValue& InputValue)
{
	const FVector ActionValue = InputValue.Get<FVector>();

	float MoveDir = ActionValue.X + ActionValue.Y;

	AddMovementInput(GetActorForwardVector(), MoveDir);

	GEngine->AddOnScreenDebugMessage(-1, 20.f,
		FColor::Red,
		FString::Printf(TEXT("x : %.5f y : %.5f"), ActionValue.X, ActionValue.Y)
	);
}