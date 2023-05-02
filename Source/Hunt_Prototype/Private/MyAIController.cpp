#include "MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMyAIController::HomePosKey(TEXT("HomePos"));
const FName AMyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMyAIController::TargetKey(TEXT("Target"));

AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_MyCharacter.BB_MyCharacter'"));
	if (BBObject.Succeeded()) {
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_MyCharacter.BT_MyCharacter'"));
	if (BTObject.Succeeded()) {
		BTAsset = BTObject.Object;
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMyAIController::RunAI()
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();				// ue5가 되면서 UBlackboardComponent*&가 TObjectPtr<UBlackboardComponent>가 되어버림
	if (UseBlackboard(BBAsset, BlackboardComp)) {
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset)) {
			HUNT_LOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AMyAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent) {
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}