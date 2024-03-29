// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "MyWeapon.h"
#include "MyCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"
#include "MyAIController.h"
#include "ABCharacterSetting.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "MyHUDWidget.h"
#include "Hunt_PrototypeGameModeBase.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SocketOffset = FVector(-15.0f, 0.0f, 200.0f);
	/*SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight + 70.0f));
	SpringArm->bUsePawnControlRotation = true;
	*/
	Camera->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PLAYER_GDC(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_PLAYER_GDC.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_PLAYER_GDC.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BP_CARDBOARD(TEXT("AnimBlueprint'/Game/Animation/RTG_ABP_MyCharacter.RTG_ABP_MyCharacter_C'"));
	if (ANIM_BP_CARDBOARD.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_BP_CARDBOARD.Class);
		HUNT_LOG(Warning, TEXT("ABP_LOAD"));
	}
	else
	{
		HUNT_LOG(Warning, TEXT("ABP_NOT_LOAD"));
	}


	SetControlMode(EControlMode::GTA);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;
	MaxCombo = 5;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));

	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded()) {
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AssetIndex = 4;
	
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();


	auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget) {
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer) {
		MyPlayerController = Cast<AMyPlayerController>(GetController());
		HUNT_CHECK(nullptr != MyPlayerController);
	}
	else {
		MyAIController = Cast<AMyAIController>(GetController());
		HUNT_CHECK(nullptr != MyAIController);	
	}

	auto DefaultSetting = GetDefault<UABCharacterSetting>();

	if (bIsPlayer) {
		auto MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
		HUNT_CHECK(nullptr != MyPlayerState);
		AssetIndex = MyPlayerState->GetCharacterIndex();
	}
	else AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	
	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (nullptr != MyGameInstance)
	{
		AssetStreamingHandle = MyGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMyCharacter::OnAssetLoadCompleted));
	}
	SetCharacterState(ECharacterState::LOADING);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	HUNT_CHECK(nullptr != MyAnim);

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn) {
			AttackStartComboState();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		HUNT_LOG(Warning, TEXT("OnHPIsZero"));
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

void AMyCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode) {
	case EControlMode::GTA:
	{
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}
	case EControlMode::DIABLO:
	{
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}
	case EControlMode::NPC:
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode) {
	case EControlMode::DIABLO:
	{
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed)); 
		break;
	}
	}

	switch (CurrentControlMode) {
	case EControlMode::DIABLO:
	{
		if (DirectionToMove.SizeSquared() > 0.0f) {
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AMyCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);

}

void AMyCharacter::MoveForward(float AxisValue)
{
	switch (CurrentControlMode) {
	case EControlMode::GTA:
	{
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
		break;
	}
	case EControlMode::DIABLO:
	{
		DirectionToMove.X = AxisValue;
		break;
	}
	}
}

void AMyCharacter::MoveRight(float AxisValue)
{
	switch (CurrentControlMode) {
	case EControlMode::GTA:
	{
		// ī�޶� �ٶ󺸴� ������ �����̱�
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
		break;
	}
	case EControlMode::DIABLO:
	{
		DirectionToMove.Y = AxisValue;
		break;
	}
	}
}

void AMyCharacter::Turn(float AxisValue)
{
	switch (CurrentControlMode) {
	case EControlMode::GTA:
	{
		AddControllerYawInput(AxisValue);
		break;
	}
	}
}

void AMyCharacter::LookUp(float AxisValue)
{
	switch (CurrentControlMode) {
	case EControlMode::GTA:
	{
		AddControllerPitchInput(AxisValue);
		break;
	}
	}
}

void AMyCharacter::ViewChange()
{
	switch (CurrentControlMode) {
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AMyCharacter::Attack()
{
	if (IsAttacking) {
		HUNT_CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo) {
			IsComboInputOn = true;
		}
	}
	else {
		HUNT_CHECK(CurrentCombo == 0);
		AttackStartComboState();
		MyAnim->PlayAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	HUNT_CHECK(IsAttacking);
	HUNT_CHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void AMyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	HUNT_CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMyCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
	if (bResult) {
		if (::IsValid(HitResult.GetActor())) {
			HUNT_LOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}

int32 AMyCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

float AMyCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float AMyCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;
	return AttackDamage * AttackModifier;
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HUNT_LOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);
	if (CurrentState == ECharacterState::DEAD) {
		if (EventInstigator->IsPlayerController()) {
			auto instigator = Cast<AMyPlayerController>(EventInstigator);
			HUNT_CHECK(nullptr != instigator, 0.0f);
			instigator->NPCKill(this);
		}
	}

	return FinalDamage;
}

bool AMyCharacter::CanSetWeapon()
{
	return true;
}

void AMyCharacter::SetWeapon(AMyWeapon* NewWeapon)
{
	HUNT_CHECK(nullptr != NewWeapon);

	if (nullptr != CurrentWeapon) {
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon) {
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMyCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	HUNT_CHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	SetCharacterState(ECharacterState::READY);
}

void AMyCharacter::SetCharacterState(ECharacterState NewState)
{
	HUNT_CHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState) {
	case ECharacterState::LOADING: {
		if (bIsPlayer) {
			DisableInput(MyPlayerController);

			MyPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
			HUNT_CHECK(nullptr != MyPlayerState);
			CharacterStat->SetNewLevel(MyPlayerState->GetCharacterLevel());
		}
		else {
			auto MyGameBase = Cast<AHunt_PrototypeGameModeBase>(GetWorld()->GetAuthGameMode());
			HUNT_CHECK(nullptr != MyGameBase);
			int32 TargetLevel = FMath::CeilToInt(((float)MyGameBase->GetScore() * 0.8f));

			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			HUNT_LOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	case ECharacterState::READY: {
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			SetCharacterState(ECharacterState::DEAD);
			});

		auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		HUNT_CHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer) {
			SetControlMode(EControlMode::GTA);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(MyPlayerController);
		}
		else {
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			MyAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD: {
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		MyAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer) DisableInput(MyPlayerController);
		else MyAIController->StopAI();

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			if (bIsPlayer) MyPlayerController->ShowResultUI();
			else Destroy();
			}), DeadTimer, false);
		break;
	}
	}
}

ECharacterState AMyCharacter::GetCharacterState() const {
	return CurrentState;
}