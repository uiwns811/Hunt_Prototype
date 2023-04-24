// Fill out your copyright notice in the Description page of Project Settings.


#include "MySection.h"
#include "MyCharacter.h"
#include "MyItemBox.h"

// Sets default values
AMySection::AMySection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("StaticMesh'/Game/StaticMesh/SM_SQUARE.SM_SQUARE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded()) {
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else {
		HUNT_LOG(Error, TEXT("Fail to load StaticMesh Asset. : %s"), *AssetPath);
	}

	// Æ®¸®°Å Ãß°¡
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	Trigger->SetCollisionProfileName(TEXT("MyTrigger"));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMySection::OnTriggerBeginOverlap);

	// Ã¶¹® ºÎÂø
	FString GateAssetPath = TEXT("StaticMesh'/Game/StaticMesh/SM_GATE.SM_GATE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		HUNT_LOG(Error, TEXT("Failed to load staticmesh asset : %s"), *GateAssetPath);
	}

	static FName GateSokets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (FName GateSocket : GateSokets)
	{
		HUNT_CHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		GateMeshes.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("MyTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMySection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	bNoBattle = false;

	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;
}

// Called when the game starts or when spawned
void AMySection::BeginPlay()
{
	Super::BeginPlay();
	
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called every frame
void AMySection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySection::SetState(ESectionState NewState)
{
	switch (NewState) {
	case ESectionState::READY:
	{
		Trigger->SetCollisionProfileName(TEXT("MyTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers) {
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(true);
		break;
	}
	case ESectionState::BATTLE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers) {
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &AMySection::OnNPCSpawn), EnemySpawnTime, false);
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			FVector2D RandXY = FMath::RandPointInCircle(600.0f);
			GetWorld()->SpawnActor<AMyItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
			}), ItemBoxSpawnTime, false);
		break;
	}
	case ESectionState::COMPLETE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers) {
			GateTrigger->SetCollisionProfileName(TEXT("MyTrigger"));
		}
		OperateGates(true);
		break;
	}
	}
	CurrentState = NewState;
}

void AMySection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes) {
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void AMySection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AMySection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentState == ESectionState::READY) {
		SetState(ESectionState::BATTLE);
	}
}

void AMySection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherCmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HUNT_CHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName)) return;

	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam);

	if (!bResult) {
		auto NewSection = GetWorld()->SpawnActor<AMySection>(NewLocation, FRotator::ZeroRotator);
	}
	else {
		HUNT_LOG(Warning, TEXT("New Section area is not empty"));
	}
}

void AMySection::OnNPCSpawn()
{
	GetWorld()->SpawnActor<AMyCharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);
}