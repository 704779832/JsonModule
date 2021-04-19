// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/CsvHelper.h"
#include "Engine.h"
//#include "../../JsonModule/Public/JsonHelper.h"
FString UCsvHelper::CsvStrConvertToJsonStr(FString csvRaw)
{
	if (csvRaw.IsEmpty())return FString();
	TArray<FString> temArray, temArray2;
	csvRaw.ParseIntoArrayLines(temArray);
	for (auto tem : temArray) {
		TArray<FString> arrays;
		tem.ParseIntoArray(arrays, TEXT(","));
		temArray2.Add(ConvertArrayToJsonArrayStr(arrays));
	}
	return ConvertArrayToJsonArrayStr(temArray2,true);
}

FString UCsvHelper::ConvertArrayToJsonArrayStr(TArray<FString> arrays, bool isRaw)
{
	if (arrays.Num() <= 0)return FString();
	FString out;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR> > > JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&out);
	JsonWriter->WriteArrayStart();
	if (isRaw)	
		for (auto tem : arrays)
			JsonWriter->WriteRawJSONValue(tem);
	else
	for (auto tem : arrays)
		JsonWriter->WriteValue(*tem);	
	JsonWriter->WriteArrayEnd();
	JsonWriter->Close();
	return out;
}
