#include "NBCPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

ANBCPawn::ANBCPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->InitCapsuleSize(42.f, 96.f);
	Capsule->SetSimulatePhysics(false);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
	Mesh->SetSimulatePhysics(false);

	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ANBCPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;

	if (IMC_Default)
	{
		Subsystem->AddMappingContext(IMC_Default, 0);
	}
}

void ANBCPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!MoveInput.IsNearlyZero())
	{
		const float ForwardValue = MoveInput.Y;
		const float RightValue = MoveInput.X;

		const FVector LocalDelta(ForwardValue, RightValue, 0.f);
		const FVector MoveDelta = LocalDelta.GetClampedToMaxSize(1.f) * MoveSpeed * DeltaSeconds;

		FHitResult Hit;
		AddActorLocalOffset(MoveDelta, true, &Hit);
	}
	
	if (!LookInput.IsNearlyZero())
	{
		const float YawDelta = LookInput.X * TurnSpeedYaw * DeltaSeconds;
		AddActorLocalRotation(FRotator(0.f, YawDelta, 0.f));

		CurrentPitch = FMath::Clamp(CurrentPitch + (-LookInput.Y * TurnSpeedPitch * DeltaSeconds), MinPitch, MaxPitch);
		SpringArm->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
	}
}

void ANBCPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;

	if (IA_Move)
	{
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ANBCPawn::OnMove);
		EIC->BindAction(IA_Move, ETriggerEvent::Completed, this, &ANBCPawn::OnMove);
	}

	if (IA_Look)
	{
		EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ANBCPawn::OnLook);
		EIC->BindAction(IA_Look, ETriggerEvent::Completed, this, &ANBCPawn::OnLook);
	}
}

void ANBCPawn::OnMove(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
}

void ANBCPawn::OnLook(const FInputActionValue& Value)
{
	LookInput = Value.Get<FVector2D>();
}