// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Input/InputData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../Skill/ShieldActor.h"
#include "PlayerAnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CreateDefaultSubobject 함수는 생성자에서만 사용 가능
	// 템플릿에 지정된 타입의 객체 하나를 생성하고 그 메모리 주소를 반환한다
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mRotation = CreateDefaultSubobject<USceneComponent>(TEXT("Rotation"));
	mRotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationMovement"));

	// 스프링암 & 카메라
	mArm->SetupAttachment(RootComponent); // SpringArm은 RootComponent의 자식으로 븉여주고

	mCamera->SetupAttachment(mArm); // Camera는 SpringArm의 자식으로 붙여준다

	mArm->TargetArmLength = 500.f;

	mArm->SetRelativeLocation(FVector(0.0, 0.0, 70.0));
	mArm->SetRelativeRotation(FRotator(-10.0, 0.0, 0.0));

	// Shield 회전
	mRotation->SetupAttachment(RootComponent); // 로테이션을 루트컴포넌트 기준으로 잡아준다

	mRotationMovement->SetUpdatedComponent(mRotation);
	mRotationMovement->RotationRate.Yaw = 180.0;


	static ConstructorHelpers::FClassFinder<AActor>BulletClassAsset(TEXT("/Script/Engine.Blueprint'/Game/Test/BP_Bullet.BP_Bullet_C'")); // 클래스타입 참조는 경로 마지막에 _C 붙여준다
	
	if (BulletClassAsset.Succeeded()) mBulletClass = BulletClassAsset.Class;

	bUseControllerRotationYaw = true; // true 일 때, 폰의 Yaw가 컨트롤러 Yaw 로테이션과 매칭된다

	// 부모로부터 상속받은 회전 값의 상속을 끊음
	mArm->bInheritPitch = false;
	mArm->bInheritYaw = true;
	mArm->bInheritRoll = false;

	GetCharacterMovement()->JumpZVelocity = 550.f;
}

void APlayerCharacter::SetMoveEnable(bool Enable)
{
	// 상태이상이 아직 유지될 경우 이 함수는 동작을 안시켜준다

	mMoveEnable = Enable;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 스프링암의 상대 회전정보 미리 초기화
	mCameraRotation = mArm->GetRelativeRotation();

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance()); // 플레이어는 AnimInst 하나를 가지게 됨

	// GetController로 컨트롤러를 찾고 PlayerController로 형변환

	// PlayerController로부터 LocalPlayer를 얻어옴
	// 미리 PlayerController를 얻어두고, nullptr이 아니면(찾음) LocalPlayer을 얻어 옴
	APlayerController* PlayerController = GetController<APlayerController>();
	if (nullptr != PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		// LocalPlayer를 이용해서 EnhancedInputLocalPlayerSubsystem을 얻어온다
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
	// Move function
	EnhancedInput->BindAction(InputData->mMove, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);

	// Rotation function
	EnhancedInput->BindAction(InputData->mRotation, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRotation);

	// CameraZoom function
	EnhancedInput->BindAction(InputData->mCameraZoom, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraZoom);

	// Attack function
	EnhancedInput->BindAction(InputData->mAttack, ETriggerEvent::Started, this, &APlayerCharacter::OnAttack);

	// Shield function
	EnhancedInput->BindAction(InputData->mShield, ETriggerEvent::Started, this, &APlayerCharacter::OnShield);

	// Jump function
	EnhancedInput->BindAction(InputData->mJump, ETriggerEvent::Started, this, &APlayerCharacter::OnJump);

}

void APlayerCharacter::OnMove(const FInputActionValue& InputValue)
{
	const FVector ActionValue = InputValue.Get<FVector>();

	if (!mMoveEnable) return;

	AddMovementInput(GetActorForwardVector(), ActionValue.Y);

	AddMovementInput(GetActorRightVector(), ActionValue.X);

	if(IsValid(mAnimInst)) mAnimInst->ComputeMoveDir(ActionValue);

	/*
	GEngine->AddOnScreenDebugMessage(-1, 20.f,
		FColor::Red,
		FString::Printf(TEXT("x : %.5f y : %.5f"), ActionValue.X, ActionValue.Y)
	);
	*/
}

void APlayerCharacter::OnAttack(const FInputActionValue& InputValue)
{
	/* 
	* 불렛 발사 코드
	FVector Location = GetActorLocation() + GetActorForwardVector() * 100.f;

	GetWorld()->SpawnActor<AActor>(mBulletClass, Location, GetActorRotation());
	*/

	PlayAttack(); // 가상 함수 호출
	mMoveEnable = false;
}

void APlayerCharacter::OnShield(const FInputActionValue& InputValue)
{
	// 실드 스킬을 사용할 수 있는지 판단하고 사용할 수 없을 경우 바로 함수를 빠져나간다
	if (mShieldEnable == false) return;

	// 사용 불가능 상태로 초기화
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

	//AddControllerYawInput(ActionValue.X);

	float PitchDelta = 90.0 * GetWorld()->GetDeltaSeconds() * ActionValue.Y;
	float YawDelta = 90.0 * GetWorld()->GetDeltaSeconds() * ActionValue.X;

	mCameraRotation.Pitch += PitchDelta;
	mCameraRotation.Yaw += YawDelta;
	//mCameraRotation.Yaw = mArm->GetRelativeRotation().Yaw;

	//FRotator Rot = mArm->GetRelativeRotation();
	// 	

	if (mCameraRotation.Yaw < -180.f) {
		// Yaw = -180.9 일 경우 180.0 - (180.9 - 180.0) = 0.9
		mCameraRotation.Yaw = 360.f + mCameraRotation.Yaw;
	}
	else if (mCameraRotation.Yaw > 180.f) {

		mCameraRotation.Yaw = mCameraRotation.Yaw - 360.f;
	}

	if (mCameraRotation.Pitch < -89.9f) {
		mCameraRotation.Pitch = -89.9f;
	}
	else if (mCameraRotation.Pitch > 89.9f) {
		mCameraRotation.Pitch = 89.9f;
	}

	mArm->SetRelativeRotation(mCameraRotation);

	//mAnimInst->ComputeAOData(ActionValue);
	mAnimInst->SetAOData(mCameraRotation.Pitch, mCameraRotation.Yaw);
}

void APlayerCharacter::OnCameraZoom(const FInputActionValue& InputValue)
{
	const float ActionValue = InputValue.Get<float>();

	mArm->TargetArmLength += ActionValue * 5.f;
}

void APlayerCharacter::OnJump(const FInputActionValue& InputValue)
{
	if (CanJump())
	{
		Jump();
		mAnimInst->OnJump();
	}
}

void APlayerCharacter::PlayAttack()
{

}

void APlayerCharacter::Attack()
{

}
