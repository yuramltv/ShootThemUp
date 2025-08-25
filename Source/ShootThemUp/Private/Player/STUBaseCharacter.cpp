// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STU_CharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInt)
    : Super(ObjInt.SetDefaultSubobjectClass<USTU_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArm);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComp");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    if (!GetWorld()) return;
    if (const auto* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }

        auto* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
        check(Input);
        Input->BindAction(MoveForward, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::OnMoveForward);
        Input->BindAction(MoveRight, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::OnMoveRight);
        Input->BindAction(LookUp, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::OnLookUp);
        Input->BindAction(TurnAround, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::OnTurnAround);
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        Input->BindAction(RunAction, ETriggerEvent::Started, this, &ASTUBaseCharacter::OnStartRunning);
        Input->BindAction(RunAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::OnStopRunning);
    }
}

void ASTUBaseCharacter::OnMoveForward(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), InputValue);

    bIsMovingForward = InputValue > 0.0f;
}

void ASTUBaseCharacter::OnMoveRight(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0f) return;
    AddMovementInput(GetActorRightVector(), InputValue);
}

void ASTUBaseCharacter::OnLookUp(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    AddControllerPitchInput(InputValue);
}

void ASTUBaseCharacter::OnTurnAround(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    AddControllerYawInput(InputValue);
}

void ASTUBaseCharacter::OnStartRunning(const FInputActionValue& Value)
{
    bWantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning(const FInputActionValue& Value)
{
    bWantsToRun = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;

    const auto VelocityNormalized = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormalized));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormalized);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}
void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = GetVelocity().Z * -1;
    UE_LOG(BaseCharacterLog, Display, TEXT("Velocity on landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return;

    const auto FinalFallDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("Fall damage: %f"), FinalFallDamage);
    TakeDamage(FinalFallDamage, FDamageEvent{}, nullptr, nullptr);
}