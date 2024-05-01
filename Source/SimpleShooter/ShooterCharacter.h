// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input (only for Player Controller)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	void Shoot();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	float RotationRate = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float Health = 0.0f;

	UFUNCTION()
	void OnPointDamageTaken(AActor* DamagedActor, float Damage, 
		AController* InstigatedBy, FVector HitLocation, 
		UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, 
		const UDamageType* DamageType, AActor* DamageCauser);

private:
	enum InputAction
	{
		IA_MoveForward = 0,
		IA_MoveRight,
		IA_LookUp,
		IA_LookRight,
		IA_LookUpRate,
		IA_LookRightRate,
		IA_Jump,
		IA_Shoot,

		IA_Num
	};

	using InputActionNameArray = FStringView[IA_Num];
	using InputActionValueArray = TStaticArray<FInputActionValue, IA_Num>;

	static const InputActionNameArray InputActionNames;

	InputActionValueArray InputActionValues = InputActionValueArray(InPlace, 0.0f);

	TObjectPtr<AGun> Gun;

	void UpdateInputs(const FInputActionInstance& Instance, int32 InputIndex);

	void UpdatePlayerMovement(float DeltaTime);
};
