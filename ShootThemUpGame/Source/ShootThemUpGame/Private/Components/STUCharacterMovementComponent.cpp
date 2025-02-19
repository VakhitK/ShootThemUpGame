// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();

    if (auto Player = Cast<ASTUBaseCharacter>(GetPawnOwner()))
    {
        return Player->IsSprinting() ? MaxSpeed * SprintModifier : MaxSpeed;
    }

    return MaxSpeed;
}