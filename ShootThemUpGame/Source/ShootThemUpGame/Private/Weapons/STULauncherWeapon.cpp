// Shoot Them Up Game. All Rights Reserved.

#include "Weapons/STULauncherWeapon.h"
#include "Weapons/STUProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(LogLauncherWeapon, All, All);

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}
void ASTULauncherWeapon::StopFire() {}

void ASTULauncherWeapon::MakeShot()
{
    if (auto&& World = GetWorld())
    {
        FVector TraceStart, TraceEnd;
        if (!GetTraceData(TraceStart, TraceEnd)) return;

        FHitResult HitResult;
        MakeHit(HitResult, TraceStart, TraceEnd);

        if (StrangeAngleCheck(HitResult)) return;

        const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
        const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

        const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
        // set projectile properties
        if (auto&& Projectile = World->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform))
        {
            Projectile->SetShotDirection(Direction);
            Projectile->SetOwner(GetOwner());
            Projectile->FinishSpawning(SpawnTransform);
        }
    }
}