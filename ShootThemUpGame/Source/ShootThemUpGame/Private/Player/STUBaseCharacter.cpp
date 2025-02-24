// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"

#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/DamageEvents.h"
#include "Components//STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(WeaponComponent);

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

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookVertical", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookHorizontal", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASTUBaseCharacter::SprintStart);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASTUBaseCharacter::SprintStop);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);
}

bool ASTUBaseCharacter::IsSprinting() const
{
    return WantsToSprint && IsMovingForward && !GetVelocity().IsZero();
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

void ASTUBaseCharacter::MoveForward(float Amount)
{
    if (Amount != 0.0f)
    {
        IsMovingForward = true;
        AddMovementInput(GetActorForwardVector(), Amount);
    }
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Amount);
    }
}

void ASTUBaseCharacter::SprintStart()
{
    WantsToSprint = true;
}

void ASTUBaseCharacter::SprintStop()
{
    WantsToSprint = false;
}

void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    // Remove character from scene after 5 seconds
    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {

        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASTUBaseCharacter::OnHealthChanged(float Value) const
{
    const auto HealthText = FText::FromString(FString::SanitizeFloat(Value, 0));
    HealthTextComponent->SetText(HealthText);
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
