// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

ALMABaseWeapon* ULMAWeaponComponent::GetWeaponObject() const
{
	if (Weapon)
	{
		return Weapon;
	}
	return nullptr;
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
		return true;
	}
	return false;
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
			Rate = Weapon->GetFireRate();
		}
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}
void ULMAWeaponComponent::Fire()
{
	if (Weapon && !AnimReloading)
	{

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Fire method called!"));

		Weapon->Fire();
	}
	else
	{
		// ≈сли анимаци€ перезар€дки активна или оружие отсутствует
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cannot fire: Weapon is null or reloading!"));
	}
}

void ULMAWeaponComponent::NoFire()
{
	if (Weapon)
	{
		Weapon->NoFire();
	}
}
void ULMAWeaponComponent::StartFiring()
{
	bIsFiring = true;

	// ќтладочное сообщение
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Started firing!"));
	if (!AnimReloading)
	{
	OneShoot();
	}

	Fire();

	// GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ULMAWeaponComponent::Fire, Rate, true);
}

void ULMAWeaponComponent::StopFiring()
{
	bIsFiring = false;
	NoFire();

	// GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsFiring && Weapon && Weapon->IsCurrentClipEmpty())
	{
		Reload();
		FireAfterReload = true;
	}
	if (!AnimReloading && bIsFiring && FireAfterReload)
	{
		FireAfterReload = false;
		Fire();
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && Weapon->CanReload();
}

void ULMAWeaponComponent::Reload()
{
	ClipEmmpty();
}

void ULMAWeaponComponent::ClipEmmpty()
{
	if (!CanReload())
		return;
	AnimReloading = true;

	NoFire();
	Weapon->ChangeClip();
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}
void ULMAWeaponComponent::OneShoot() {
	Weapon->Shoot();
}