// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class ULMAWeaponComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArmComponent")
	float ArmLengthMin = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArmComponent")
	float ArmLengthMax = 2800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArmComponent")
	int CameraZoomStep = 100;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float Stamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float SprintCost = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float StaminaRecoveryRate = 1.3f;

	UPROPERTY(BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting;

	UFUNCTION(BlueprintCallable)
	bool IsSprinting() const;

	FTimerHandle TimerHandle;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float YRotation = -75.0f;

	float ArmLength = 1400.0f;

	float FOV = 55.0f;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void CameraZoomIn();
	void CameraZoomOut();

	void OnDeath();
	void OnHealthChanged(float NewHealth);

	void RotationPlayerOnCursor();

	bool IsMovingForward();

	void BeginSprint();
	void EndSprint();
	void ControlStamina();
};