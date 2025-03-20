// Shoot Them Up Game. All Rights Reserved.

#include "AI/Services/STUChangeWeaponService.h"
#include "STUAIWeaponComponent.h"
#include "AIController.h"
#include "STUUtils.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (auto&& Controller = OwnerComp.GetAIOwner();
        auto&& WeaponComponent = STUUtils::GetComponent<USTUWeaponComponent>(Controller->GetPawn()))
    {
        if (Probability > 0.0f && FMath::FRand() <= Probability)
        {
            WeaponComponent->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}