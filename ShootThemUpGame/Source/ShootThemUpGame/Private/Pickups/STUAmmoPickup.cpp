// Shoot Them Up Game. All Rights Reserved.

#include "Pickups/STUAmmoPickup.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(const APawn* Pawn)
{
    UE_LOG(LogAmmoPickup, Display, TEXT("Ammo picked up!"));

    const auto HealthComponent = STUUtils::GetComponent<USTUHealthComponent>(Pawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = STUUtils::GetComponent<USTUWeaponComponent>(Pawn);
    if (!WeaponComponent) return false;

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}