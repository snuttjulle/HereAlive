// Copyright (C) Snutt Treptow - All Rights Reserved


#include "KIAChatManager.h"
#include "Components/TwitchPlayComponent.h"

AKIAChatManager::AKIAChatManager()
{
	mTwitchPlayComponent = CreateDefaultSubobject<UTwitchPlayComponent>("TwitchPlay");

	MinimumTimeBetweenWateringRequests = 10;
}

void AKIAChatManager::ExecuteWaterCommand(FString command, TArray<FString> attr, FString username)
{
	const int minTimeBetweenRequests = MinimumTimeBetweenWateringRequests; // in minutes
	const FDateTime now = FDateTime::Now();

	if (!UserRequestTimes.Contains(username))
	{
		UserRequestTimes.Add(username, now + FTimespan::FromMinutes(minTimeBetweenRequests));
	}

	// If a user exists in the request queue then ignore it
	if (UserRequestTimes.Contains(username)) // if this user has made a request in the past make sure their next request isn't close enough in time
	{
		FDateTime lastRequestTime = UserRequestTimes[username];
		FTimespan timeElapsed = now - lastRequestTime;

		if ( timeElapsed.GetMinutes() < 0 || timeElapsed.GetMinutes() >= minTimeBetweenRequests)
		{
			RequestQueue.Add(username);
			UserRequestTimes[username] = FDateTime::Now();
			OnWaterCommand.Broadcast(username);

			UE_LOG(LogTemp, Display, TEXT("%s was added to pending water requests"), *username);
		}
	}
}

FString AKIAChatManager::PopWateringRequestQueue()
{
	if (RequestQueue.Num() > 0)
	{
		FString username = RequestQueue[0];
		RequestQueue.RemoveAtSwap(0);

		OnWateringRequestIssued.Broadcast(username);
		
		UE_LOG(LogTemp, Display, TEXT("Popped watering request for %s at %s"), *username, *FDateTime::Now().ToString());
		
	return username;
	}
	else
	{
		return "";
	}
}
