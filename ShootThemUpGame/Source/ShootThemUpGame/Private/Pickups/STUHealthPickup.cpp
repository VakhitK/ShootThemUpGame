// Shoot Them Up Game. All Rights Reserved.

#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(const APawn* Pawn)
{
    UE_LOG(LogHealthPickup, Display, TEXT("Health picked up!"));

    const auto HealthComponent = STUUtils::GetComponent<USTUHealthComponent>(Pawn);
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}