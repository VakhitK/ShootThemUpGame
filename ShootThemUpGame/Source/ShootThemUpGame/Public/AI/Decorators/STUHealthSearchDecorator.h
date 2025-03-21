// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUHealthSearchDecorator.generated.h"

UCLASS()
class SHOOTTHEMUPGAME_API USTUHealthSearchDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USTUHealthSearchDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.6f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
