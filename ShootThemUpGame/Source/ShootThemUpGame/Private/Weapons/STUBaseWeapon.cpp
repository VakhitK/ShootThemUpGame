// Shoot Them Up Game. All Rights Reserved.

#include "Weapons/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(UIData.MainIcon, TEXT("Main icon for weapon not set!"));
    checkf(UIData.CrossHairIcon, TEXT("Crosshair for weapon not set!"));
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets couldn't be less or equal 0!"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips couldn't be less or equal 0!"));

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    if (!GetWorld()) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return false;

    if (Player->IsPlayerControlled())
    {
        const auto Controller = GetPlayerController();
        if (!Controller) return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }

    return true;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    CurrentAmmo.Bullets--;

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}
void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("---------- Out of Clips!!! ----------"));
            return;
        }

        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(LogBaseWeapon, Display, TEXT("---------- Change Clip ----------"));
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

    if (IsAmmoEmpty())
    {
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }

    return true;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo : " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += IsClipEmpty() ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() const
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,  //
        WeaponMesh,                                                //
        MuzzleSocketName,                                          //
        FVector::ZeroVector,                                       //
        FRotator::ZeroRotator,                                     //
        EAttachLocation::SnapToTarget,                             //
        true);
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::StrangeAngleCheck(const FHitResult& HitResult) const
{
    const auto SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector ActualTraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
    const FVector HitDirectionFromMuzzle = (ActualTraceEnd - SocketTransform.GetLocation()).GetSafeNormal();
    const float Degrees =
        FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SocketTransform.GetRotation().GetForwardVector(), HitDirectionFromMuzzle)));

    UE_LOG(LogBaseWeapon, Display, TEXT("ShotAngle :%f"), Degrees);

    return Degrees > StrangeAngleValue;
}
