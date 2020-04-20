// Copyright (C) Snutt Treptow - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "KIAChatManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaterCommandDelegate, FString, Username); // Event issued when a user has successfully been added to pending list of watering
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWateringRequestIssued, FString, Username); // Event issued when a pending watering request has been issued and is handled

/**
 * 
 */
UCLASS()
class KEEPITALIVE_API AKIAChatManager : public AActor
{
    GENERATED_BODY()

public:
    AKIAChatManager();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TwitchChat")
    class UTwitchPlayComponent* mTwitchPlayComponent;

    UFUNCTION(BlueprintCallable, Category = "TwitchChat")
    void ExecuteWaterCommand(FString command, TArray<FString> attr, FString username);

	UFUNCTION(BlueprintCallable, Category = "TwitchChat")
    FORCEINLINE bool HasPendingWateringRequests() const { return RequestQueue.Num() > 0; }

    // Pops the request queue, if there are no pending requests it will return empty string
	UFUNCTION(BlueprintCallable, Category = "TwitchChat")
	FString PopWateringRequestQueue();

protected:
    // The amount of minutes that needs to pass inbetween requests to water the flower
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
    int32 MinimumTimeBetweenWateringRequests;

private:
    UPROPERTY(BlueprintAssignable, Category = "TwitchChat")
    FWaterCommandDelegate OnWaterCommand;

	UPROPERTY(BlueprintAssignable, Category = "TwitchChat")
	FWateringRequestIssued OnWateringRequestIssued;

    UPROPERTY()
    TArray<FString> RequestQueue;

	UPROPERTY()
    TMap<FString, FDateTime> UserRequestTimes;
};
