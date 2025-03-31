// Shoot Them Up Game. All Rights Reserved.

#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    const UObject* QueryOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(QueryOwner, QueryInstance.QueryID);

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const ASTUBasePickup* Pickup = Cast<ASTUBasePickup>(ItemActor);
        if (!Pickup) continue;

        It.SetScore(TestPurpose, FilterType, Pickup->CouldBeTaken(), BoolValue.GetValue());
    }

    Super::RunTest(QueryInstance);
}