// Shoot Them Up Game. All Rights Reserved.

#include "AI/Decorators/STUHealthSearchDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "STUHealthComponent.h"

USTUHealthSearchDecorator::USTUHealthSearchDecorator()
{
    NodeName = "Health Search";
}

bool USTUHealthSearchDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HealthComponent = STUUtils::GetComponent<USTUHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() <= HealthPercent;
}