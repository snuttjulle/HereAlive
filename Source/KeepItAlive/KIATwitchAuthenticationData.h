// Copyright (C) Snutt Treptow - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "KIATwitchAuthenticationData.generated.h"

USTRUCT(BlueprintType)
struct FTwitchAuthenticationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Oauth token for user that acts as a bot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AuthData")
	FString OauthToken;

	// Username of bot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuthData")
	FString Username;

	// Channel to connect to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuthData")
	FString ChannelName;
};