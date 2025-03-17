// Shoot Them Up Game. All Rights Reserved.

#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;
    if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavLocation))
    {
        Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
        return Super::ExecuteTask(OwnerComp, NodeMemory);
    }

    return EBTNodeResult::Failed;
}