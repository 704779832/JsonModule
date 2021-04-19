#pragma once
#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H
#include "Engine.h"
#ifndef MYPRINTLOG
#define MYPRINTLOG
namespace MyTools {
	static void printLog(FString name) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *name);
	}
}
#endif
namespace MyTools {

	/*字符串中根据两个特定的字符找出里面的子串数组
	*source 原字符串
	*L 左关键字符
	*R 右关键字符
	*/
	static TArray<FString> FindSubStrByLR(const FString source, const FString L, const FString R) {
		static TArray<FString> saveArray;
		int i = 0, j = 0;
		i = source.Find(L);
		j = source.Find(R, ESearchCase::IgnoreCase, ESearchDir::FromStart, i + 1);
		if (i < 0 || j < 0) {
			TArray<FString> out;
			for (auto tem : saveArray) {
				out.Add(tem);
			}
			saveArray.Empty();
			return out;
		}
		else {
			saveArray.Push(source.Mid(i + 1, j - i - 1));
			return FindSubStrByLR(source.Mid(j + 1, source.Len() - j - 1), L, R);
		}
	}
	/*字符串对生成JsonObject*/
	static FString StringToJson(TMap<FString, FString> data) {
		FString res;
		TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR> > > JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&res);
		JsonWriter->WriteObjectStart();
		for (auto tem : data) {
			JsonWriter->WriteValue(tem.Key, tem.Value);
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
		return res;
	}

	/*解析Json*/
	static bool ParseJson(TSharedPtr<FJsonValue>& jsonValue, FString json) {
		TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(json);
		return FJsonSerializer::Deserialize(JsonReader, jsonValue);
	}
	/*生成Json*/
	static FString CreateJson(const TSharedPtr<FJsonObject>& jsonValue) {
		FString out;
		auto tem = TJsonWriterFactory<>::Create(&out);
		FJsonSerializer::Serialize(jsonValue.ToSharedRef(), tem);
		return out;
	}
	static FString CreateJson(const TArray<TSharedPtr<FJsonValue>> arrays) {
		FString out;
		auto tem = TJsonWriterFactory<>::Create(&out);
		FJsonSerializer::Serialize(arrays, tem);
		return out;
	}
	/*获取JsonValue值*/
	static FString ParseJsonValueSingle(TSharedPtr<FJsonValue> jsonValue) {
		switch (jsonValue->Type) {
		case EJson::String:
			return jsonValue->AsString();
			break;
		case EJson::Number:
			return FString::SanitizeFloat(jsonValue->AsNumber());
			break;
		case EJson::Boolean:
			return jsonValue->AsBool() ? TEXT("true") : TEXT("false");
		case EJson::Array:
			return CreateJson(jsonValue->AsArray());
			break;
		case EJson::Object:
			return CreateJson(jsonValue->AsObject());
			break;
		}
		return FString();
	}
}
#endif // !STRINGTOOLS_H
