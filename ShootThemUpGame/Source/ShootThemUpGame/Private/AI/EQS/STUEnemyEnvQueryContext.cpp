// Shoot Them Up Game. All Rights Reserved.

#include "AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    if (!QueryOwner) return;

    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;

    if (auto&& EnemyActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKeyName)))
    {
        UEnvQueryItemType_Actor::SetContextHelper(ContextData, EnemyActor);
    }
}