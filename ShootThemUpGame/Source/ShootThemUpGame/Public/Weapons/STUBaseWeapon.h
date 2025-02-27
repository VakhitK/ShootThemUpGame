// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

UCLASS()
class SHOOTTHEMUPGAME_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    virtual void StartFire() {}
    virtual void StopFire() {}

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float StrangeAngleValue = 90.0;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void MakeShot() {}
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    FVector GetMuzzleWorldLocation() const;
    bool StrangeAngleCheck(const FHitResult& HitResult) const;
    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
};
