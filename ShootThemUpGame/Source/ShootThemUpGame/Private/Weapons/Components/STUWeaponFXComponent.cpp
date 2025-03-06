// Shoot Them Up Game. All Rights Reserved.

#include "Weapons/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    const auto Effect =
        Hit.PhysMaterial.IsValid() && EffectsMap.Contains(Hit.PhysMaterial.Get()) ? EffectsMap[Hit.PhysMaterial.Get()] : DefaultEffect;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}

void USTUWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();
}
