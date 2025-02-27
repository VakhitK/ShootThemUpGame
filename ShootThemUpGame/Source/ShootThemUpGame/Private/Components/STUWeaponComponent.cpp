// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StartFire();
    }
}
void USTUWeaponComponent::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void USTUWeaponComponent::NextWeapon()
{
    CurrentWeaponIndex = (++CurrentWeaponIndex) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (const auto& Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    UWorld* World = GetWorld();
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (World && Character)
    {

        for (const auto& Weapon : WeaponClasses)
        {
            auto WeaponInstance = World->SpawnActor<ASTUBaseWeapon>(Weapon);
            if (!WeaponInstance) continue;

            WeaponInstance->SetOwner(Character);
            Weapons.Push(WeaponInstance);

            AttachWeaponToSocket(WeaponInstance, Character->GetMesh(), WeaponArmorySocketName);
        }
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName)
{
    if (!Weapon || !Mesh) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(Mesh, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || WeaponClasses.IsEmpty()) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    if (auto&& Character = Cast<ACharacter>(GetOwner()))
    {
        Character->PlayAnimMontage(Animation);
    }
}

void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage) return;

    for (const auto& NotifyEvent : EquipAnimMontage->Notifies)
    {
        if (auto&& EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify))
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    if (auto&& Character = Cast<ACharacter>(GetOwner()); Character->GetMesh() == MeshComp)
    {
        UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
    }
}
