// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "../Input/TankInputData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../Skill/ShieldActor.h"
#include "../Skill/Bullet.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	mBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	SetRootComponent(mBody);
	mBodyMesh->SetupAttachment(mBody);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Test/Body.Body'"));
	if (BodyMeshAsset.Succeeded()) mBodyMesh->SetStaticMesh(BodyMeshAsset.Object);

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

	mMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	mMuzzle->SetupAttachment(mBarrelMesh);


	// �޽� ����
	mBodyMesh->SetWorldScale3D(FVector(2.0, 2.0, 1.0));
	mHeadMesh->SetRelativeScale3D(FVector(0.5, 0.5, 1.0));
	mHeadMesh->SetRelativeLocation(FVector(0.0, 0.0, 100));
	mBarrelMesh->SetRelativeScale3D(FVector(2.0, 0.25, 0.25));
	mBarrelMesh->SetRelativeLocation(FVector(50, 0, 0));
	mMuzzle->SetRelativeLocation(FVector(115.0, 0, 0));

	// FloatingPawnComponent �߰� (������ �����ϰ�)
	mMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(mBody);

	// �������� & ī�޶�
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mArm->SetupAttachment(mBodyMesh); // SpringArm�� RootComponent�� �ڽ����� �ٿ��ְ�
	mCamera->SetupAttachment(mArm); // Camera�� SpringArm�� �ڽ����� �ٿ��ش�

	mArm->TargetArmLength = 500.f;
	mArm->SetRelativeLocation(FVector(0.0, 0.0, 80.0));
	mArm->SetRelativeRotation(FRotator(-30.0f, 0.0, 0.0));

	// ȸ��
	bUseControllerRotationYaw = true; // true �϶�, ���� Yaw�� ��Ʈ�ѷ� Yaw �����̼ǰ� ��Ī�ȴ� 

	// �ڽ�������Ʈ ������ ����
	mBody->SetBoxExtent(FVector(100.0, 100.0, 50.0));

	// �ڽ�������Ʈ�� Player CollisionProfile�� �����ϰ� ������ �޽��� �浹ó�� �ȵǰ� ����
	mBody->SetCollisionProfileName(TEXT("Player"));
	mBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mHeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mBarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	const FVector ActionValue = InputValue.Get<FVector>();

	AddMovementInput(GetActorForwardVector(), ActionValue.Y);

	AddControllerYawInput(ActionValue.X);
}

void APlayerPawn::OnAttack(const FInputActionValue& InputValue)
{
	// GetComponentLocation(), GetComponentRotation() : ������Ʈ�� ���� ��ġ, ȸ������ ������ �Լ�
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), mMuzzle->GetComponentLocation(), mMuzzle->GetComponentRotation());

	Bullet->SetOwnerController(GetController()); // �÷��̾ �� �Ѿ˿� ���� �´´ٸ� �÷��̾ �� ���̶�� ���� ����
}

void APlayerPawn::OnRotation(const FInputActionValue& InputValue)
{
	const FVector ActionValue = InputValue.Get<FVector>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	mHeadMesh->AddRelativeRotation(FRotator(0.0, 60.0 * DeltaTime * ActionValue.X, 0.0)); // Yaw ȸ�� Mouse X�� �ش�
	mBarrelMesh->AddRelativeRotation(FRotator(60.0 * DeltaTime * ActionValue.Y, 0.0, 0.0)); // Pitch ȸ�� MouseY�� �ش�

}

