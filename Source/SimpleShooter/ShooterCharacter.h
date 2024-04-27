// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "ShooterCharacter.generated.h"

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

private:
	enum InputAction
	{
		IA_MoveForward = 0,
		IA_MoveRight,
		IA_LookUp,
		IA_LookRight,
		IA_Jump,

		IA_Num
	};

	using InputActionNameArray = FStringView[IA_Num];
	using InputActionValueArray = TStaticArray<FInputActionValue, IA_Num>;

	static const InputActionNameArray InputActionNames;

	InputActionValueArray InputActionValues = InputActionValueArray(InPlace, 0.0f);

	void UpdateInputs(const FInputActionInstance& Instance, int32 InputIndex);

	void UpdateMovement();
};
