// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/Actor.h"
#include "MySection.generated.h"

UCLASS()
class HUNT_PROTOTYPE_API AMySection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySection();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		TArray<UStaticMeshComponent*> GateMeshes;

	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		TArray<UBoxComponent*> GateTriggers;

	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
		bool bNoBattle;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
		float EnemySpawnTime;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
		float ItemBoxSpawnTime;

	FTimerHandle SpawnNPCTimerHandle = {};
	FTimerHandle SpawnItemBoxTimerHandle = {};

private:
	enum class ESectionState : uint8 {
		READY = 0,
		BATTLE,
		COMPLETE
	};

	void SetState(ESectionState NewState);
	ESectionState CurrentState = ESectionState::READY;

	void OperateGates(bool bOpen = true);

	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnNPCSpawn();

	UFUNCTION()
		void OnKeyNPCDestroyed(AActor* DestroyedActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
