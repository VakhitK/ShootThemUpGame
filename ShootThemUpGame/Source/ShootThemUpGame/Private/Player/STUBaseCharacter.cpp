// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"

#include "STUWeaponComponent.h"
#include "Components/InputComponent.h"
#include "Engine/DamageEvents.h"
#include "Components//STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(WeaponComponent);
    checkf(GetMesh(), TEXT("Character should have mesh for ragdoll!"));

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& color) const
{
    if (!GetMesh()) return;

    const auto DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!DynamicMaterial) return;

    DynamicMaterial->SetVectorParameterValue(MaterialColorName, color);
}

// UKismetAnimationLibrary::CalculateDirection
float ASTUBaseCharacter::GetMovementDirection() const
{
    const auto Velocity = GetVelocity();

    if (!Velocity.IsNearlyZero())
    {
        const FMatrix RotMatrix = FRotationMatrix(GetActorRotation());
        const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
        const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
        const FVector NormalizedVel = Velocity.GetSafeNormal2D();

        // get a cos(alpha) of forward vector vs velocity
        const float ForwardCosAngle = static_cast<float>(FVector::DotProduct(ForwardVector, NormalizedVel));
        // now get the alpha and convert to degree
        float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));
        // depending on where right vector is, flip it
        const float RightCosAngle = static_cast<float>(FVector::DotProduct(RightVector, NormalizedVel));

        if (RightCosAngle < 0.f)
        {
            ForwardDeltaDegree *= -1.f;
        }

        return ForwardDeltaDegree;
    }

    return 0.f;
}

void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    // PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    // Remove character from scene after 5 seconds
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChanged(float Value, float Delta) const {}

void ASTUBaseCharacter::OnStartFire()
{
    if (IsSprinting()) return;
    WeaponComponent->StartFire();
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;

    UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X)
    {
        return;
    }

    const auto Damage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("LandDamage: %f"), Damage);

    TakeDamage(Damage, FDamageEvent{}, nullptr, Hit.GetActor());
}
