// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUPGAME_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUWeaponComponent();

    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();

    bool IsFiring() const { return CurrentWeapon && CurrentWeapon->IsFiring(); }
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
    bool TryToAddAmmo(const TSubclassOf<ASTUBaseWeapon>& WeaponType, int32 ClipsAmount);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponsData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    bool CanFire() const { return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress; }
    bool CanEquip() const { return !EquipAnimInProgress && !ReloadAnimInProgress; }
    void EquipWeapon(int32 WeaponIndex);

    int32 CurrentWeaponIndex = 0;

private:
    void SpawnWeapons();
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName);
    bool CanReload() const { return CurrentWeapon && CurrentWeapon->CanReload() && !ReloadAnimInProgress && !EquipAnimInProgress; }

    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;
};
