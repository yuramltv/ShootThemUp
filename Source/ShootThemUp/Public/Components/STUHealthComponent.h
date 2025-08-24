// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    float GetHealth() const { return Health; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    float Health = 0.0f;
};
