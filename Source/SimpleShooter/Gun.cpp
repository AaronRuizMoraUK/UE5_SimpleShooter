// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	if (GunTrace(HitResult, ShotDirection))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());

		if (auto* HitActor = HitResult.GetActor())
		{
			//UE_LOG(LogTemp, Display, TEXT("Gun %s hits %s with Damage %0.2f from %s"),
			//	*GetName(), *HitActor->GetName(), Damage, *ShotDirection.ToString());

			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AGun::GetOwnerController()
{
	if (auto* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		return OwnerPawn->GetController();
	}
	return nullptr;
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	auto* OwnerController = GetOwnerController();
	if (!OwnerController)
	{
		return false;
	}

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	ShotDirection = ViewPointRotation.Vector();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(TArray<AActor*>{ this, GetOwner() });

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,
		ViewPointLocation,
		ViewPointLocation + ViewPointRotation.Vector() * MaxRange,
		ECC_GameTraceChannel1,
		QueryParams);

	//if (bHit)
	//{
	//	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
	//	DrawDebugDirectionalArrow(GetWorld(), 
	//		HitResult.ImpactPoint, 
	//		HitResult.ImpactPoint + HitResult.ImpactNormal * 100.0f, 
	//		100.0f, FColor::Blue, false, 3.0f);
	//}

	return bHit;
}
