// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	//Speed = 0.0f;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	APawn* Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
	}
	else return;
	Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
}