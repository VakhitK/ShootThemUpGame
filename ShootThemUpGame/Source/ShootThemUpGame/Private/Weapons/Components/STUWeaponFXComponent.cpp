// Shoot Them Up Game. All Rights Reserved.

#include "Weapons/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    const auto ImpactData = Hit.PhysMaterial.IsValid() && ImpactDataMap.Contains(Hit.PhysMaterial.Get())
                                ? ImpactDataMap[Hit.PhysMaterial.Get()]
                                : DefaultImpactData;

    // Niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),  //
        ImpactData.NiagaraEffect,                               //
        Hit.ImpactPoint,                                        //
        Hit.ImpactNormal.Rotation());

    // Decal
    const auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),  //
        ImpactData.DecalData.Material,                                     //
        ImpactData.DecalData.Size,                                         //
        Hit.ImpactPoint,                                                   //
        Hit.ImpactNormal.Rotation());

    if (Decal)
    {
        Decal->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}

void USTUWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();
}
