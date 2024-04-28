// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

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

	if (auto* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (auto* OwnerController = OwnerPawn->GetController())
		{
			FVector ViewPointLocation;
			FRotator ViewPointRotation;
			OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

			if (FHitResult HitResult;
				GetWorld()->LineTraceSingleByChannel(HitResult,
					ViewPointLocation,
					ViewPointLocation + ViewPointRotation.Vector() * MaxRange,
					ECC_GameTraceChannel1))
			{
				DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
			}
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
