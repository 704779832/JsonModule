// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CsvHelper.generated.h"

/**
 * 
 */
UCLASS()
class UCsvHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	UFUNCTION(BlueprintPure, Category = "Csv")
	static FString CsvStrConvertToJsonStr(FString csvRaw);

	static FString ConvertArrayToJsonArrayStr(TArray<FString> arrays,bool isRaw=false);
	
};
