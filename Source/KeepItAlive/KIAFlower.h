// Copyright (C) Snutt Treptow - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KIAFlower.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlowerDiedDelegate);

UCLASS()
class KEEPITALIVE_API AKIAFlower : public AActor
{
	GENERATED_BODY()
	
public:	
	AKIAFlower();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Flower")
	FORCEINLINE bool IsAlive() const { return mCurrentHealth > 0.f; }

	UFUNCTION(BlueprintPure, Category = "Flower")
	FORCEINLINE float GetFlowerCurrentHealth() const { return mCurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Flower")
	FORCEINLINE float GetFlowerMaxHealth() const { return mMaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Flower")
	void WaterFlower();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mSpeedScale;

	// Total health of the flower (excluding upper health buffer)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mMaxHealth;

	// Once the plants current health exceeds max health it will buffer this amount of health (e-tanks in super metroid ish)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mHealthBuffer;

	// How long it will take for the flower to whither in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mDegradationTime;

	// How fast the plant can absorb water in the soil in water units per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mSoilAbsorbtionSpeed;

	// Units of water per second that is absorbed by the flower
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mUnitsPerWatering;

	// the amount of water the soil will absorb in itself
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float mSoilBuffer;

	// Amount that the soil has soaked up itself that won't go to the flower
	UPROPERTY(Transient)
	float mCurrentlySoakedUpWaterInSoil;

	// What the plants current health is at
	UPROPERTY(Transient)
	float mCurrentHealth;

	// The amount of water that's currently in the soil
	UPROPERTY(Transient)
	float mWaterCurrentlyInSoil;

private:
	void UpdateTimeDebugging(float deltaTime);

	UPROPERTY(BlueprintAssignable, Category = "Flower")
	FFlowerDiedDelegate OnFlowerDied;
};
