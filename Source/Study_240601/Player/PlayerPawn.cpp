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

	// 스프링암 & 카메라
	mArm->SetupAttachment(mBodyMesh); // SpringArm은 RootComponent의 자식으로 붙여주고
	mCamera->SetupAttachment(mArm); // Camera는 SpringArm의 자식으로 붙여준다

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

		// LocalPlayer를 이용해서 EnhancedInputLocalPlayerSubsystem을 얻어옴
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		// UTankInputData의 CDO를 꺼내온다
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

	// Cast : 언리얼 UObject 객체들의 형변환 함수
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// UDefaultInputData의 CDO를 꺼내온다
	const UTankInputData* InputData = GetDefault<UTankInputData>();

	// 원하는 InputAction이 동작할 때 호출될 함수의 함수 포인터를 지정
	// Move 함수
	EnhancedInput->BindAction(InputData->mMove, ETriggerEvent::Triggered, this, &APlayerPawn::OnMove);

	// Rotation 함수
	EnhancedInput->BindAction(InputData->mRotation, ETriggerEvent::Triggered, this, &APlayerPawn::OnRotation);

	// Attack 함수
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

