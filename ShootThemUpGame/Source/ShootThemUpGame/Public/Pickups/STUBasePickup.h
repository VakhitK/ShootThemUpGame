// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUPGAME_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

public:
    ASTUBasePickup();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f;

    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
    virtual void Tick(float DeltaTime) override;
    bool CouldBeTaken() const { return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle); }

private:
    virtual bool GivePickupTo(const APawn* Pawn) { return false; }
    void PickupWasTaken();
    void Respawn();
    void GenerateRotationYaw();

    float RotationYaw = 0.0f;
    FTimerHandle RespawnTimerHandle;
};
