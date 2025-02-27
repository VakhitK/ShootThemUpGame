// Shoot Them Up Game. All Rights Reserved.

#include "Weapons/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    if (!GetWorld()) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
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
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
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
