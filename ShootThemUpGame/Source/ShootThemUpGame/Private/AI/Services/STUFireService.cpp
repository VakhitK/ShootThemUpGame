// Shoot Them Up Game. All Rights Reserved.

#include "STUFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (auto&& Controller = OwnerComp.GetAIOwner();
        auto&& WeaponComponent = STUUtils::GetComponent<USTUWeaponComponent>(Controller->GetPawn()))
    {
        HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}