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

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

    AActor* ComponentOwner = GetOwner();
    if (!ComponentOwner)
    {
        UE_LOG(LogHealthComponent, Warning, TEXT("No owner found"));
        return;
    }
    ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);

    GetWorld()->GetTimerManager().SetTimer(HasTakenDamageRecentlyHandle, this, &USTUHealthComponent::TurnOnAutoHeal, 1.0);
}
void USTUHealthComponent::TurnOnAutoHeal()
{
    UE_LOG(LogHealthComponent, Display, TEXT("turning on autoheal"));
    if (!bHasTakenDamage)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::Heal, HealUpdateRate, bAutoHeal, HealDelay);
    }
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead()) return;
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
}

void USTUHealthComponent::Heal()
{
    Health = FMath::Clamp(Health + HealModifier, HealModifier, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}