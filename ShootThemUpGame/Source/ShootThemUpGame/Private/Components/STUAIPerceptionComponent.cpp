// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if (PerceivedActors.IsEmpty()) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    AActor* ClosestEnemy = nullptr;
    float ClosestDistance = FLT_MAX;

    for (const auto& Actor : PerceivedActors)
    {
        const auto HealthComponent = STUUtils::GetComponent<USTUHealthComponent>(Actor);
        if (HealthComponent && !HealthComponent->IsDead())
        {
            const auto Distance = (Pawn->GetActorLocation() - Actor->GetActorLocation()).Size();
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestEnemy = Actor;
            }
        }
    }

    return ClosestEnemy;
}