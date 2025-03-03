// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUPlayerHudWidget.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"

float USTUPlayerHudWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;

    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return false;

    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}