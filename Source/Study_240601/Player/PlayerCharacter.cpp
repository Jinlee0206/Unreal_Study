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

	// CreateDefaultSubobject �Լ��� �����ڿ����� ����Ѵ�
	// ���ø��� ������ Ÿ���� ��ü �ϳ��� �����ϰ� �� �޸� �ּҸ� ��ȯ���ش�.
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mArm->SetupAttachment(RootComponent); // SpringArm�� RootComponent�� �ڽ����� �ٿ��ְ�

	mCamera->SetupAttachment(mArm); // Camera�� SpringArm�� �ڽ����� �ٿ��ش�

	mArm->TargetArmLength = 500.f;

	mArm->SetRelativeLocation(FVector(0.0, 0.0, 70.0));
	mArm->SetRelativeRotation(FRotator(-10.0, 0.0, 0.0));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// GetController�� ��Ʈ�ѷ��� ã�� PlayerController�� �� ��ȯ

	// PlayerController�� ���� LocalPlayer�� ����
	// �̸� PlayerController�� ��� �ΰ�, nullptr�� �ƴϸ�(ã��) LocalPlayer�� ����
	APlayerController* PlayerController = GetController<APlayerController>();
	if (nullptr != PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		// LocalPlayer�� �̿��ؼ� EnhancedInputLocalPlayerSubsystem�� ����
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		// UDefaultInputData�� CDO�� �����´�
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

	// Cast : �𸮾� UObject ��ü���� ����ȯ �Լ�
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// UDefaultInputData�� CDO�� �����´�
	const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();

	// ���ϴ� InputAction�� ������ �� ȣ��� �Լ��� �Լ� �����͸� ����
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