// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "AutoHeal")
    bool bAutoHeal{true};

    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float HealUpdateRate{0.3f};

    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float HealDelay{3.0f};

    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float HealModifier{1.0f};

    FTimerHandle HealTimerHandle;

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    float Health = 0.0f;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void Heal();
    void SetHealth(float NewHealth);
};
