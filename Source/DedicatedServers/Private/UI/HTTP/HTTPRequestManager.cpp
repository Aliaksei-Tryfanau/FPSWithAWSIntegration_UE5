// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HTTP/HTTPRequestManager.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "Player/DSLocalPlayerSubsystem.h"

UDSLocalPlayerSubsystem* UHTTPRequestManager::GetDSLocalPlayerSubsystem() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	
	if (IsValid(LocalPlayerController))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(LocalPlayerController->Player);
		
		if (IsValid(LocalPlayer))
		{
			return LocalPlayer->GetSubsystem<UDSLocalPlayerSubsystem>();
		}
	}
	
	return nullptr;
}

bool UHTTPRequestManager::ContainsErrors(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown Error");
		FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown Error Message");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Message: %s"), *ErrorMessage);
		return true;
	}
	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown Error");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		return true;
	}
	return false;
}

void UHTTPRequestManager::DumpMetaData(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		TSharedPtr<FJsonObject> MetaDataJsonObject = JsonObject->GetObjectField(TEXT("$metadata"));
		FDSMetaData DSMetaData;
		FJsonObjectConverter::JsonObjectToUStruct(MetaDataJsonObject.ToSharedRef(), &DSMetaData);
		DSMetaData.Dump();
	}
}

FString UHTTPRequestManager::SerializeJsonContent(const TMap<FString, FString>& Params)
{
	TSharedPtr<FJsonObject> ContentJsonObject = MakeShareable(new FJsonObject);
	for (const auto& Param : Params)
	{
		ContentJsonObject->SetStringField(Param.Key, Param.Value);
	}
	FString Content;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(ContentJsonObject.ToSharedRef(), JsonWriter);
	
	return Content;
}

void FDSRetrieveMatchStatsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("FDSRetrieveMatchStatsResponse:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("  highestStreak: %d"), highestStreak);
	UE_LOG(LogDedicatedServers, Log, TEXT("  dethroneElims: %d"), dethroneElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  gotFirstBlood: %d"), gotFirstBlood);
	UE_LOG(LogDedicatedServers, Log, TEXT("  defeats: %d"), defeats);
	UE_LOG(LogDedicatedServers, Log, TEXT("  scoredElims: %d"), scoredElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  email: %s"), *email);
	UE_LOG(LogDedicatedServers, Log, TEXT("  misses: %d"), misses);
	UE_LOG(LogDedicatedServers, Log, TEXT("  revengeElims: %d"), revengeElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  matchLosses: %d"), matchLosses);
	UE_LOG(LogDedicatedServers, Log, TEXT("  showstopperElims: %d"), showstopperElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  databaseid: %s"), *databaseid);
	UE_LOG(LogDedicatedServers, Log, TEXT("  headShotElims: %d"), headShotElims);
	UE_LOG(LogDedicatedServers, Log, TEXT("  username: %s"), *username);
	UE_LOG(LogDedicatedServers, Log, TEXT("  hits: %d"), hits);
	UE_LOG(LogDedicatedServers, Log, TEXT("  matchWins: %d"), matchWins);
}
