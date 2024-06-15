// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Input/InputData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../Skill/ShieldActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CreateDefaultSubobject �Լ��� �����ڿ����� ����Ѵ�
	// ���ø��� ������ Ÿ���� ��ü �ϳ��� �����ϰ� �� �޸� �ּҸ� ��ȯ���ش�.
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mRotation = CreateDefaultSubobject<USceneComponent>(TEXT("Rotation"));
	mRotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationMovement"));

	// �������� & ī�޶�
	mArm->SetupAttachment(RootComponent); // SpringArm�� RootComponent�� �ڽ����� �ٿ��ְ�

	mCamera->SetupAttachment(mArm); // Camera�� SpringArm�� �ڽ����� �ٿ��ش�

	mArm->TargetArmLength = 500.f;

	mArm->SetRelativeLocation(FVector(0.0, 0.0, 70.0));
	mArm->SetRelativeRotation(FRotator(-10.0, 0.0, 0.0));

	// Shield ȸ��
	mRotation->SetupAttachment(RootComponent); // �����̼���

	mRotationMovement->SetUpdatedComponent(mRotation);
	mRotationMovement->RotationRate.Yaw = 180.0;


	static ConstructorHelpers::FClassFinder<AActor>BulletClassAsset(TEXT("/Script/Engine.Blueprint'/Game/Test/BP_Bullet.BP_Bullet_C'")); // ��� �������� _C �ٿ��ֱ�
	
	if (BulletClassAsset.Succeeded()) mBulletClass = BulletClassAsset.Class;

	bUseControllerRotationYaw = true; // true �϶�, ���� Yaw�� ��Ʈ�ѷ� Yaw �����̼ǰ� ��Ī�ȴ� 

	// �θ�κ��� ��ӹ��� ȸ������ ����� ����
	mArm->bInheritPitch = false;
	mArm->bInheritYaw = true;
	mArm->bInheritRoll = false;
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

	// �ǵ� ��ų�� ��� �Ұ��� ������ ��� CoolDown ���
	if (mShieldEnable == false)
	{
		mShieldTime += DeltaTime;
		if (mShieldTime >= mShieldCoolDown)
		{
			mShieldEnable = true;
			mShieldTime = 0;
		}
	}
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
	// Move �Լ�
	EnhancedInput->BindAction(InputData->mMove, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);

	// Rotation �Լ�
	EnhancedInput->BindAction(InputData->mRotation, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRotation);

	// CameraZoom �Լ�
	EnhancedInput->BindAction(InputData->mCameraZoom, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraZoom);

	// Attack �Լ�
	EnhancedInput->BindAction(InputData->mAttack, ETriggerEvent::Started, this, &APlayerCharacter::OnAttack);

	// Shield �Լ�
	EnhancedInput->BindAction(InputData->mShield, ETriggerEvent::Started, this, &APlayerCharacter::OnShield);

}

void APlayerCharacter::OnMove(const FInputActionValue& InputValue)
{
	const FVector ActionValue = InputValue.Get<FVector>();

	AddMovementInput(GetActorForwardVector(), ActionValue.Y);

	AddMovementInput(GetActorRightVector(), ActionValue.X);

	/*
	GEngine->AddOnScreenDebugMessage(-1, 20.f,
		FColor::Red,
		FString::Printf(TEXT("x : %.5f y : %.5f"), ActionValue.X, ActionValue.Y)
	);
	*/
}

void APlayerCharacter::OnAttack(const FInputActionValue& InputValue)
{
	FVector Location = GetActorLocation() + GetActorForwardVector() * 100.f;

	GetWorld()->SpawnActor<AActor>(mBulletClass, Location, GetActorRotation());
}

void APlayerCharacter::OnShield(const FInputActionValue& InputValue)
{
	// �ǵ� ��ų�� ����� �� �ִ��� �Ǵ��ϰ� ����� �� ���� ��� �ٷ� �Լ��� ����������
	if (mShieldEnable == false) return;

	// ��� �Ұ��� ���·� ����
	mShieldEnable = false;

	// ���� ������ �̸� ����� �θ� ��ӽ�Ű�� ���
	FVector Location[4];
	Location[0] = GetActorLocation() + GetActorForwardVector() * 200.f;
	Location[1] = GetActorLocation() + GetActorRightVector() * 200.f;
	Location[2] = GetActorLocation() + GetActorForwardVector() * -200.f;
	Location[3] = GetActorLocation() + GetActorRightVector() * -200.f;

	for (int32 i = 0; i < 4; i++)
	{
		FRotator Rot = GetActorRotation() + FRotator(0.0, i * 90.0, 0.0);

		// StaticClass() : UClass ������ ����
		AShieldActor* Shield = GetWorld()->SpawnActor<AShieldActor>(AShieldActor::StaticClass(), Location[i], Rot);

		// Shield�� �θ�� �÷��̾ �����Ѵ�
		// Shield->AttachToActor : �θ�� ������ Actor�� RootComponent�� �ٿ��ش�
		// Shield->AttachToComponent : ���ϴ� ������Ʈ�� �����Ͽ� �ٿ��ش�

		// KeepWorldTransform : ���� ������ �����ϸ� �ٿ��ش�
		// KeepRelativeTransform : ��� ������ �����ϸ� �ٿ��ش�
		Shield->AttachToComponent(mRotation, FAttachmentTransformRules::KeepWorldTransform); // ���� ������ �����ϸ鼭 �θ� �ٿ��ֱ�
		
	}
}

void APlayerCharacter::OnRotation(const FInputActionValue& InputValue)
{
	const FVector ActionValue = InputValue.Get<FVector>();

	AddControllerYawInput(ActionValue.X);

	mArm->AddRelativeRotation(FRotator(90.0 * GetWorld()->GetDeltaSeconds() * ActionValue.Y, 0.0, 0.0));
}

void APlayerCharacter::OnCameraZoom(const FInputActionValue& InputValue)
{
	const float ActionValue = InputValue.Get<float>();

	mArm->TargetArmLength += ActionValue * 5.f;
}
