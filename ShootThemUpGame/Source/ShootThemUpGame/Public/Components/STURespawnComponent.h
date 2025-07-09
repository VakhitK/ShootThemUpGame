// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUPGAME_API USTURespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTURespawnComponent();

    void Respawn(int32 RespawnTime);
    int32 GetRespawnCountdown() const { return RespawnCountdown; }
    bool IsRespawnInProgress() const;

private:
    void RespawnTimerUpdate();

    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountdown = 0;
};
