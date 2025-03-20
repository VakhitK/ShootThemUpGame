// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUPGAME_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    virtual void StartFire() { FireInProgress = true; }
    virtual void StopFire() { FireInProgress = false; }
    void ChangeClip();
    bool IsAmmoEmpty() const;
    bool CanReload() const { return (CurrentAmmo.Bullets < DefaultAmmo.Bullets) && CurrentAmmo.Clips > 0; }
    bool IsFiring() const { return FireInProgress; }
    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }
    bool TryToAddAmmo(int32 ClipsAmount);

    FOnClipEmptySignature OnClipEmpty;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float StrangeAngleValue = 90.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void MakeShot() {}
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    FVector GetMuzzleWorldLocation() const;
    bool StrangeAngleCheck(const FHitResult& HitResult) const;
    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    void DecreaseAmmo();
    void LogAmmo();
    bool IsClipEmpty() const;
    bool IsAmmoFull() const;

    UNiagaraComponent* SpawnMuzzleFX() const;

private:
    FAmmoData CurrentAmmo;
    bool FireInProgress = false;
};
