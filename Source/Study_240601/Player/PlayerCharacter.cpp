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

	// CreateDefaultSubobject 함수는 생성자에서만 사용한다
	// 템플릿에 지정된 타입의 객체 하나를 생성하고 그 메모리 주소를 반환해준다.
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mRotation = CreateDefaultSubobject<USceneComponent>(TEXT("Rotation"));
	mRotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationMovement"));

	// 스프링암 & 카메라
	mArm->SetupAttachment(RootComponent); // SpringArm은 RootComponent의 자식으로 붙여주고

	mCamera->SetupAttachment(mArm); // Camera는 SpringArm의 자식으로 붙여준다

	mArm->TargetArmLength = 500.f;

	mArm->SetRelativeLocation(FVector(0.0, 0.0, 70.0));
	mArm->SetRelativeRotation(FRotator(-10.0, 0.0, 0.0));

	// Shield 회전
	mRotation->SetupAttachment(RootComponent); // 로테이션을

	mRotationMovement->SetUpdatedComponent(mRotation);
	mRotationMovement->RotationRate.Yaw = 180.0;


	static ConstructorHelpers::FClassFinder<AActor>BulletClassAsset(TEXT("/Script/Engine.Blueprint'/Game/Test/BP_Bullet.BP_Bullet_C'")); // 경로 마지막에 _C 붙여주기
	
	if (BulletClassAsset.Succeeded()) mBulletClass = BulletClassAsset.Class;

	bUseControllerRotationYaw = true; // true 일때, 폰의 Yaw가 컨트롤러 Yaw 로테이션과 매칭된다 

	// 부모로부터 상속받은 회전값의 상속을 끊음
	mArm->bInheritPitch = false;
	mArm->bInheritYaw = true;
	mArm->bInheritRoll = false;
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

	// 실드 스킬이 사용 불가능 상태일 경우 CoolDown 계산
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

	// Cast : 언리얼 UObject 객체들의 형변환 함수
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// UDefaultInputData의 CDO를 꺼내온다
	const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();

	// 원하는 InputAction이 동작할 때 호출될 함수의 함수 포인터를 지정
	// Move 함수
	EnhancedInput->BindAction(InputData->mMove, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);

	// Rotation 함수
	EnhancedInput->BindAction(InputData->mRotation, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRotation);

	// CameraZoom 함수
	EnhancedInput->BindAction(InputData->mCameraZoom, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraZoom);

	// Attack 함수
	EnhancedInput->BindAction(InputData->mAttack, ETriggerEvent::Started, this, &APlayerCharacter::OnAttack);

	// Shield 함수
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
	// 실드 스킬을 사용할 수 있는지 판단하고 사용할 수 없을 경우 바로 함수를 빠져나간다
	if (mShieldEnable == false) return;

	// 사용 불가능 상태로 만듬
	mShieldEnable = false;

	// 월드 정보를 미리 만들고 부모에 상속시키는 방법
	FVector Location[4];
	Location[0] = GetActorLocation() + GetActorForwardVector() * 200.f;
	Location[1] = GetActorLocation() + GetActorRightVector() * 200.f;
	Location[2] = GetActorLocation() + GetActorForwardVector() * -200.f;
	Location[3] = GetActorLocation() + GetActorRightVector() * -200.f;

	for (int32 i = 0; i < 4; i++)
	{
		FRotator Rot = GetActorRotation() + FRotator(0.0, i * 90.0, 0.0);

		// StaticClass() : UClass 정보를 얻어옴
		AShieldActor* Shield = GetWorld()->SpawnActor<AShieldActor>(AShieldActor::StaticClass(), Location[i], Rot);

		// Shield의 부모로 플레이어를 지정한다
		// Shield->AttachToActor : 부모로 지정된 Actor의 RootComponent에 붙여준다
		// Shield->AttachToComponent : 원하는 컴포넌트를 지정하여 붙여준다

		// KeepWorldTransform : 월드 정보를 유지하며 붙여준다
		// KeepRelativeTransform : 상대 정보를 유지하며 붙여준다
		Shield->AttachToComponent(mRotation, FAttachmentTransformRules::KeepWorldTransform); // 월드 정보를 유지하면서 부모에 붙여주기
		
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
