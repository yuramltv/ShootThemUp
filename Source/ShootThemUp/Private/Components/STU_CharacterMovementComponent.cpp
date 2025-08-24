// Shoot Them Up Game. All Rights Reserved.

#include "Components/STU_CharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTU_CharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const auto* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}