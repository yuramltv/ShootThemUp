// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (!ComponentOwner)
    {
        UE_LOG(LogHealthComponent, Warning, TEXT("No owner found"));
        return;
    }
    ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    // turn off auto heal on taking damage
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (bAutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::Heal, HealUpdateRate, bAutoHeal, HealDelay);
    }
}

void USTUHealthComponent::Heal()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}
void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}