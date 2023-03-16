// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG_GDC(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC'"));
	if (SK_KWANG_GDC.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_KWANG_GDC.Object);
	}

	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -150.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm->TargetArmLength = 400.0f;
	//SpringArm->SocketOffset = FVector(0f, 0.0f, 200.0f);

	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	
		/*SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight + 70.0f));
	SpringArm->bUsePawnControlRotation = true;

	Camera->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));*/

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BP_KWANG(TEXT("AnimBlueprint'/Game/Animation/ABP_MyCharacter.ABP_MyCharacter_C'"));
	if (ANIM_BP_KWANG.Succeeded())
	{
		Mesh->SetAnimInstanceClass(ANIM_BP_KWANG.Class);
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AMyPawn::PossessedBy(AController* NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyPawn::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyPawn::LookUp);
}

void AMyPawn::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
	ABLOG_S(Warning);
}

void AMyPawn::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
	ABLOG_S(Warning);
}

void AMyPawn::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AMyPawn::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}
