// Copyright (C) Snutt Treptow - All Rights Reserved


#include "KIAFlower.h"

//#define SPEED_SCALE_DEBUGGING


#ifdef SPEED_SCALE_DEBUGGING
float elapsedTime;
int elapsedTimeInHours;
int elapsedTimeInMinutes;
int elapsedTimeInSeconds;
#endif

AKIAFlower::AKIAFlower()
{
	PrimaryActorTick.bCanEverTick = true;

	mMaxHealth = 100.f;
	mHealthBuffer = 50.f;
	mDegradationTime = 60 * 60; // from minutes to seconds
	mSoilAbsorbtionSpeed = 1.f; // water units per second
	mUnitsPerWatering = 10.f;
}

void AKIAFlower::BeginPlay()
{
	Super::BeginPlay();

	mCurrentHealth = mMaxHealth;
	mCurrentlySoakedUpWaterInSoil = 0.f;
	mWaterCurrentlyInSoil = 0.f;

#ifdef SPEED_SCALE_DEBUGGING
	elapsedTime = 0.f;
	elapsedTimeInHours = 0;
	elapsedTimeInMinutes = 0;
	elapsedTimeInSeconds = 0;
#endif
}

void AKIAFlower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive())
	{
#ifdef SPEED_SCALE_DEBUGGING
		const float speedScale = FMath::Max(1.f, mSpeedScale);
		UpdateTimeDebugging(DeltaTime);
#else
		const float speedScale = 1.f;
#endif

		// apply damage with the amount of delta time in correlation to how long it's supposed to take for it to die
		mCurrentHealth -= ((DeltaTime / mDegradationTime) * mMaxHealth) * speedScale;

		// Calculate how much water there is currently in the soil
		float waterAbsorbedByFlower = (DeltaTime * mSoilAbsorbtionSpeed) * speedScale;

		if(mWaterCurrentlyInSoil > 0.f)
		{
			float vaporizationMultiplier = 1.f;
			if (mCurrentHealth >= mMaxHealth)
			{
				vaporizationMultiplier *= 2.f;
			}

			mWaterCurrentlyInSoil = FMath::Max(0.f, mWaterCurrentlyInSoil - (waterAbsorbedByFlower * vaporizationMultiplier));

			// First let the soil's buffer soak up the water
			if (mCurrentlySoakedUpWaterInSoil < mSoilBuffer)
			{
				const float amountLeft = FMath::Max(0.f, mSoilBuffer - mWaterCurrentlyInSoil);
				mCurrentlySoakedUpWaterInSoil = FMath::Min(mSoilBuffer, mCurrentlySoakedUpWaterInSoil + waterAbsorbedByFlower);

				waterAbsorbedByFlower = amountLeft;
			}

			// calculate how much water we can absorb this frame if there is water in soil that the soil hasn't soaked up
			if (waterAbsorbedByFlower > 0.f)
			{
				mCurrentHealth = FMath::Min(mCurrentHealth + waterAbsorbedByFlower, mMaxHealth + mHealthBuffer);
			}
		}
		else if (mCurrentlySoakedUpWaterInSoil > 0.f)
		{
			mCurrentlySoakedUpWaterInSoil = FMath::Max(0.f, mCurrentlySoakedUpWaterInSoil - waterAbsorbedByFlower);
		}

		// If the flower has died we broadcast this event
		if (mCurrentHealth <= 0.f)
		{
			OnFlowerDied.Broadcast();
			UE_LOG(LogTemp, Display, TEXT("Flower died at %s"), *FDateTime::Now().ToString());
		}

		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::SanitizeFloat(mCurrentlySoakedUpWaterInSoil));
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::SanitizeFloat(mWaterCurrentlyInSoil));
	}
}

void AKIAFlower::WaterFlower()
{
	mWaterCurrentlyInSoil += mUnitsPerWatering;

	// Add this line if you want to limit how much water can be added in a short amount of time
	//mWaterCurrentlyInSoil = FMath::Min(mWaterCurrentlyInSoil, mMaxHealth + mHealthBuffer);
}

#ifdef SPEED_SCALE_DEBUGGING
void AKIAFlower::UpdateTimeDebugging(float deltaTime)
{
	elapsedTime += deltaTime * mSpeedScale;

	if (elapsedTime > 1.f)
	{
		elapsedTimeInSeconds++;
		elapsedTime -= 1.f;
	}

	if (elapsedTimeInSeconds >= 60)
	{
		elapsedTimeInMinutes++;
		elapsedTimeInSeconds = 0;
	}

	if (elapsedTimeInMinutes >= 60)
	{
		elapsedTimeInHours++;
		elapsedTimeInMinutes = 0;
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("elapsed time: %s:%s:%s"), *FString::FromInt(elapsedTimeInHours), *FString::FromInt(elapsedTimeInMinutes), *FString::FromInt(elapsedTimeInSeconds)));
}
#endif