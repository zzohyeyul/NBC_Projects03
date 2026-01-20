#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NBCPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class NBC_PROJECTS03_API ANBCPawn : public APawn
{
	GENERATED_BODY()

public:
	ANBCPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* IMC_Default = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Look = nullptr;

	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category="Tuning")
	float MoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category="Tuning")
	float TurnSpeedYaw = 120.f;

	UPROPERTY(EditAnywhere, Category="Tuning")
	float TurnSpeedPitch = 90.f;

	UPROPERTY(EditAnywhere, Category="Tuning")
	float MinPitch = -70.f;

	UPROPERTY(EditAnywhere, Category="Tuning")
	float MaxPitch = 20.f;

	FVector2D MoveInput = FVector2D::ZeroVector;
	FVector2D LookInput = FVector2D::ZeroVector;

	float CurrentPitch = -10.f;
};