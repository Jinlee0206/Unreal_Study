// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "../Input/TankInputData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Head.Head'"));
	if (BodyMeshAsset.Succeeded()) mBodyMesh->SetStaticMesh(BodyMeshAsset.Object);

	SetRootComponent(mBodyMesh);


	// Create the HeadMesh component and attach it to the BodyMesh
	mHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	mHeadMesh->SetupAttachment(mBodyMesh);

	// Load the Head Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HeadMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Head.Head'"));
	if (HeadMeshAsset.Succeeded())
	{
		mHeadMesh->SetStaticMesh(HeadMeshAsset.Object);
	}

	// Create the BarrelMesh component and attach it to the HeadMesh
	mBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	mBarrelMesh->SetupAttachment(mHeadMesh);

	// Load the Barrel Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Barrel.Barrel'"));
	if (BarrelMeshAsset.Succeeded())
	{
		mBarrelMesh->SetStaticMesh(BarrelMeshAsset.Object);
	}

	mBodyMesh->SetWorldScale3D(FVector(0.5, 0.5, 1.0));
	mHeadMesh->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
	mHeadMesh->SetRelativeLocation(FVector(0.0, 0.0, 100));
	mBarrelMesh->SetRelativeScale3D(FVector(0.25, 2.0, 0.25));
	mBarrelMesh->SetRelativeLocation(FVector(0, 50.0, 0));


	mMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(mBodyMesh);

	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// �������� & ī�޶�
	mArm->SetupAttachment(mBodyMesh); // SpringArm�� RootComponent�� �ڽ����� �ٿ��ְ�
	mCamera->SetupAttachment(mArm); // Camera�� SpringArm�� �ڽ����� �ٿ��ش�

	mArm->TargetArmLength = 500.f;
	mArm->SetRelativeLocation(FVector(0.0, 0.0, 80.0));
	mArm->SetRelativeRotation(FRotator(-10.0f, 0.0, 0.0));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetController<APlayerController>();
	if (nullptr != PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		// LocalPlayer�� �̿��ؼ� EnhancedInputLocalPlayerSubsystem�� ����
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		// UTankInputData�� CDO�� �����´�
		const UTankInputData* InputData = GetDefault<UTankInputData>();

		Subsystem->AddMappingContext(InputData->mTankContext, 0);
	}
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast : �𸮾� UObject ��ü���� ����ȯ �Լ�
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// UDefaultInputData�� CDO�� �����´�
	const UTankInputData* InputData = GetDefault<UTankInputData>();

	// ���ϴ� InputAction�� ������ �� ȣ��� �Լ��� �Լ� �����͸� ����
	// Move �Լ�
	EnhancedInput->BindAction(InputData->mMove, ETriggerEvent::Triggered, this, &APlayerPawn::OnMove);

	// Rotation �Լ�
	EnhancedInput->BindAction(InputData->mRotation, ETriggerEvent::Triggered, this, &APlayerPawn::OnRotation);

	// Attack �Լ�
	EnhancedInput->BindAction(InputData->mAttack, ETriggerEvent::Started, this, &APlayerPawn::OnAttack);

}

void APlayerPawn::OnMove(const FInputActionValue& InputValue)
{
}

void APlayerPawn::OnAttack(const FInputActionValue& InputValue)
{
}

void APlayerPawn::OnRotation(const FInputActionValue& InputValue)
{
	
}

