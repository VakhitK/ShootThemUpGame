// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class ASTUProjectile;

UCLASS()
class SHOOTTHEMUPGAME_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUProjectile> ProjectileClass;

private:
    virtual void MakeShot() override;
};
