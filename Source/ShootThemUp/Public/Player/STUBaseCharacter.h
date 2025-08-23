// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInt);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputMappingContext* InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UInputAction> MoveForward;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UInputAction> RunAction;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UInputAction> MoveRight;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UInputAction> LookUp;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UInputAction> TurnAround;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction* JumpAction;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    bool bWantsToRun = false;
    bool bIsMovingForward = false;

    void OnMoveForward(const FInputActionValue& Value);
    void OnMoveRight(const FInputActionValue& Value);
    void OnLookUp(const FInputActionValue& Value);
    void OnTurnAround(const FInputActionValue& Value);
    void OnStartRunning(const FInputActionValue& Value);
    void OnStopRunning(const FInputActionValue& Value);
};