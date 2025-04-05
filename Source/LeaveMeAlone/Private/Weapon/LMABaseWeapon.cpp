// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::Fire()
{
	if (!IsCurrentClipEmpty())
	{
		// Отладочное сообщение
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Fire method in baseWeapon called!"));
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ALMABaseWeapon::Shoot, AmmoWeapon.FireRate, true);
	}
}

void ALMABaseWeapon::NoFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Shoot()
{
	// Отладочное сообщение
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Shoot!"));
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector	 TraceStart = SocketTransform.GetLocation();
	const FVector	 ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector	 TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::White, false, 1.0f);
	}

	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets()
{
	if (!IsCurrentClipEmpty())
	{
		CurrentAmmoWeapon.Bullets--;
		UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	}
	if (IsCurrentClipEmpty())
	{
		ClipEmpty.Broadcast();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

void ALMABaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
bool ALMABaseWeapon::CanReload() const
{
	return !IsClipFull() && CurrentAmmoWeapon.Bullets < AmmoWeapon.Bullets;
}
