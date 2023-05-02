// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class HUNT_PROTOTYPE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AMyWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UMyCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	FOnAttackEndDelegate OnAttackEnd;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY()
		class UMyAnimInstance* MyAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;

	UPROPERTY()
		class AMyAIController* MyAIController;

	UPROPERTY()
		class AMyPlayerController* MyPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;

	FTimerHandle DeadTimerHandle = {};

public:
	// Sets default values for this character's properties
	AMyCharacter();

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	
	int32 GetExp() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode {
		GTA,
		DIABLO,
		NPC
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	bool CanSetWeapon();
	void SetWeapon(class AMyWeapon* NewWeapon);

	void Attack();

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);

	void ViewChange();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	void OnAssetLoadCompleted();
};
