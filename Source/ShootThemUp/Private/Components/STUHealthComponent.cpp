// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUHealthComponent.h"

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
