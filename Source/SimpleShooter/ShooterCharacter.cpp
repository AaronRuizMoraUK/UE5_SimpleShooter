// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

const AShooterCharacter::InputActionNameArray AShooterCharacter::InputActionNames =
{
	TEXT("IA_MoveForward"),
	TEXT("IA_MoveRight"),
	TEXT("IA_LookUp"),
	TEXT("IA_LookRight"),
	TEXT("IA_LookUpRate"),
	TEXT("IA_LookRightRate"),
	TEXT("IA_Jump"),
	TEXT("IA_Shoot")
};

AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(TEXT("weapon_l"), EPhysBodyOp::PBO_None);

	if (GunClass)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass, GetActorLocation(), GetActorRotation());
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("weapon_rSocket"));
		Gun->SetOwner(this); // Important for other systems like damage or multiplayer.
	}

	Health = MaxHealth;

	OnTakePointDamage.AddDynamic(this, &AShooterCharacter::OnPointDamageTaken);
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If character is controlled by Player
	if (Cast<APlayerController>(GetController()))
	{
		//for (int32 i = 0; i < IA_Num; ++i)
		//{
		//	UE_LOG(LogTemp, Display, TEXT("- %.*s: %f"), InputActionNames[i].Len(), InputActionNames[i].GetData(), InputActionValues[i].Get<float>());
		//}

		UpdatePlayerMovement(DeltaTime);

		if (InputActionValues[IA_Jump].Get<bool>())
		{
			Jump();
		}

		if (InputActionValues[IA_Shoot].Get<bool>())
		{
			Shoot();
		}
	}
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Obtain Player Controller
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player controller found!"));
		return;
	}

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not using enhanced input component!"));
		return;
	}

	// Load Input Mapping Context asset
	auto* InputMappingData = InputMapping.LoadSynchronous();
	if (!InputMappingData)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid Input Mapping Context!"));
		return;
	}

	// Add input mapping context to the input system.
	if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		InputSystem->ClearAllMappings();
		InputSystem->AddMappingContext(InputMappingData, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No enhanced input local player subsystem found!"));
		return;
	}

	auto GetActionItem = [InputMappingData](FStringView actionItemName) -> const UInputAction*
		{
			auto* foundMapping = InputMappingData->GetMappings().FindByPredicate(
				[&actionItemName](const FEnhancedActionKeyMapping& mapping)
				{
					return mapping.Action->GetName() == actionItemName;
				});

			if (foundMapping)
			{
				return foundMapping->Action.Get();
			}

			UE_LOG(LogTemp, Warning, TEXT("Action Item %.*s not found!"), actionItemName.Len(), actionItemName.GetData());
			return nullptr;
		};

	for (int32 i = 0; i < IA_Num; ++i)
	{
		EnhancedInputComponent->BindAction(GetActionItem(InputActionNames[i]), ETriggerEvent::Triggered, this, &AShooterCharacter::UpdateInputs, i);
		EnhancedInputComponent->BindAction(GetActionItem(InputActionNames[i]), ETriggerEvent::Completed, this, &AShooterCharacter::UpdateInputs, i);
	}
}

bool AShooterCharacter::IsAlive() const
{
	return Health > 0.0f;
}

void  AShooterCharacter::Shoot()
{
	if (Gun)
	{
		Gun->PullTrigger();
	}
}

void AShooterCharacter::OnPointDamageTaken(AActor* DamagedActor, float Damage, 
	AController* InstigatedBy, FVector HitLocation, 
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Display, TEXT("Actor %s (%s) hit by %s with Damage %0.2f from %s"), 
	//	*GetName(), *DamagedActor->GetName(), *DamageCauser->GetName(), Damage, *ShotFromDirection.ToString());

	Health = FMath::Max(0.0f, Health - Damage);
	UE_LOG(LogTemp, Display, TEXT("%s health %.2f"), *GetName(), Health);
}

void AShooterCharacter::UpdateInputs(const FInputActionInstance& Instance, int32 InputIndex)
{
	InputActionValues[InputIndex] = Instance.GetValue();

	//UE_LOG(LogTemp, Display, TEXT("Input triggered '%.*s' with value %.2f"), 
	//	InputActionNames[InputIndex].Len(), InputActionNames[InputIndex].GetData(), InputActionValues[InputIndex].Get<float>());
}

void AShooterCharacter::UpdatePlayerMovement(float DeltaTime)
{
	AddMovementInput(GetActorForwardVector(), InputActionValues[IA_MoveForward].Get<float>());
	AddMovementInput(GetActorRightVector(), InputActionValues[IA_MoveRight].Get<float>());

	AddControllerPitchInput(InputActionValues[IA_LookUp].Get<float>());
	AddControllerYawInput(InputActionValues[IA_LookRight].Get<float>());

	AddControllerPitchInput(InputActionValues[IA_LookUpRate].Get<float>() * RotationRate * DeltaTime);
	AddControllerYawInput(InputActionValues[IA_LookRightRate].Get<float>() * RotationRate * DeltaTime);
}
