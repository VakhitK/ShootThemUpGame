// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components//STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"

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

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto HealthText = FText::FromString(FString::SanitizeFloat(HealthComponent->GetHealth(), 0));
    HealthTextComponent->SetText(HealthText);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookVertical", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookHorizontal", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASTUBaseCharacter::SprintStart);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASTUBaseCharacter::SprintStop);
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
