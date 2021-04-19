// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "JsonHelper.generated.h"

UENUM(BlueprintType)
enum class EJsonType : uint8 {
	None,
	Null,
	String,
	Number,
	Boolean,
	Array,
	Object
};


USTRUCT(BlueprintType)
struct FJsonStruct
{
	GENERATED_USTRUCT_BODY()
	TSharedPtr<FJsonValue> value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString key;
};
UCLASS()
class UJsonHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/*解析字符串*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static FJsonStruct ParseJson(FString str);
	UFUNCTION(BlueprintPure, Category = "Json")
		static bool JsonIsValid(const FJsonStruct& In);
	/*json生成字符串*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static FString ToString(FJsonStruct json);
	/*获取类型*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static EJsonType JsonType(FJsonStruct json);
	/*尝试获取Json基础类型value*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static FString TryGetBaseValue(FJsonStruct json);
	/*获取JSON类型为Object和Array中的所有value*/
	UFUNCTION(BlueprintPure, Category = "Json")
	static TArray<FJsonStruct> GetAllValueFromJson(FJsonStruct json);
	/*查找Json类型为Object中的特定Value*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Find"), Category = "Json")
		static FJsonStruct FindJsonFromObject(FJsonStruct json, FString key, EJsonType type,bool& isFound);
		static bool FindJsonFromObject_C(TSharedPtr<FJsonValue> json, FString key,EJson type, TSharedPtr<FJsonValue>& result);
	/*设置Json Key值*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static FJsonStruct SetJsonKey(FJsonStruct json,FString key);
	/*设置Json Value*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void ChangeJsonArrayValue(UPARAM(ref)FJsonStruct& json, FJsonStruct from, FJsonStruct to, bool& isSuccess);
	UFUNCTION(BlueprintPure, Category = "Json")
		static void ChangeJsonObjectValue(FJsonStruct json, FString key, FJsonStruct value, bool& isSuccess);


	/*从Array中移除某一项*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void RemoveJsonArrayValueByValue(FJsonStruct json, FJsonStruct value,bool& isSuccess);
	/*从Objec中移除某一项*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void RemoveJsonObjectValueByKey(FJsonStruct json, FString key, bool& isSuccess);
	/*增加Array一个元素*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void AddJsonArrayValue(UPARAM(ref) FJsonStruct& json, FJsonStruct value, bool& isSuccess);
	/*增加Object一个元素*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static void AddJsonObjectValue(FJsonStruct json, FJsonStruct value, bool& isSuccess,bool isRepleace = true);

	/*生成基础类型*/
	UFUNCTION(BlueprintPure, Category = "Json")
		static FJsonStruct CreateJsonStringValue(FString key, FString value);
	UFUNCTION(BlueprintPure, Category = "Json")
		static FJsonStruct CreateJsonNumberValue(FString key, float value);
	UFUNCTION(BlueprintPure, Category = "Json")
		static FJsonStruct CreateJsonBoolValue(FString key, bool value);
	/*生成空Object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateEmptyJsonObject"), Category = "Json")
		static FJsonStruct CreateJsonObject();
	/*将多个Json合并到一个Object中 返回的Json类型为Object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateJsonObject"), Category = "Json")
		static FJsonStruct CreateJsonObjectByValue(TArray<FJsonStruct> arrays);
	static void CreateJsonObjectValue_C(TSharedPtr<FJsonObject> obj, FJsonStruct addItem);

	/*生成空Array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateEmptyJsonArray"), Category = "Json")
		static FJsonStruct CreateJsonArray();
	/*将多个Json合并到一个Array中 返回的Json类型为Array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CreateJsonArray"), Category = "Json")
		static FJsonStruct CreateJsonArrayByValue(TArray<FJsonStruct> arrays);
	static void CreateJsonArrayValue_C(TArray<TSharedPtr<FJsonValue>>& ary, FJsonStruct addItem);


	/*将json object转换为键值对*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Key-Value Pairs"), Category = "Json")
		static FString JsonObjectToKeyValuePairs(FJsonStruct json);

	/*类型转换*/
	/*将String 转换为bool*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToBool (FString)",BlueprintAutocast), Category = "Utilities|String")
		static bool Conv_StringToBool(const FString& InString);
	/*将FVector 转换为json object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonObject (FVector)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_VectorToJsonObject(FVector InFVector,FString Xkey=TEXT("x"), FString Ykey = TEXT("y"), FString Zkey = TEXT("z"));
	/*将FVector 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (FVector)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_VectorToJsonArray(FVector InFVector);
	/*将json object 转换为FVector*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector (JsonObject)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FVector Conv_JsonObjecttToVector(FJsonStruct InFJsonStruct, FString Xkey = TEXT("x"), FString Ykey = TEXT("y"), FString Zkey = TEXT("z"));
	/*将json array 转换为FVector*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector (JsonArray)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FVector Conv_JsonArraytToVector(FJsonStruct InFJsonStruct);
	/*将FRotator 转换为json object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonObject (FRotator)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_RotatorToJsonObject(FRotator InFRotator, FString RollKey= TEXT("roll"), FString PitchKey= TEXT("pitch"), FString YawKey= TEXT("yaw"));
	/*将FRotator 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (FRotator)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_RotatorToJsonArray(FRotator InFRotator);

	/*将FQuat 转换为json object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonObject (FQuat)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_QuatToJsonObject(FQuat In, FString xKey = TEXT("x"), FString yKey = TEXT("y"), FString zKey = TEXT("z"), FString wKey = TEXT("w"));
	/*将FQuat 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (FQuat)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_QuatToJsonArray(FQuat In);

	/*将json object 转换为FRotator*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRotator (JsonObject)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FRotator Conv_JsonObjecttToRotator(FJsonStruct InFJsonStruct, FString Xkey = TEXT("Roll"), FString Ykey = TEXT("Pitch"), FString Zkey = TEXT("Yaw"));
	/*将json array 转换为FRotator*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRotator (JsonArray)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FRotator Conv_JsonArraytToRotator(FJsonStruct InFJsonStruct);

	/*将FVector2D 转换为json object*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonObject (FVector2D)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_Vector2DToJsonObject(FVector2D InFVector2D, FString Xkey = TEXT("x"), FString Ykey = TEXT("y"));
	/*将FVector2D 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (FVector2D)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_Vector2DToJsonArray(FVector2D InFVector2D);
	/*将json object 转换为FVector2D*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector2D (JsonObject)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FVector2D Conv_JsonObjecttToVector2D(FJsonStruct InFJsonStruct, FString Xkey = TEXT("x"), FString Ykey = TEXT("y"));
	/*将json array 转换为FVector2D*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector2D (JsonArray)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static FVector2D Conv_JsonArraytToVector2D(FJsonStruct InFJsonStruct);

	/*将json array 转换为FString*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToStringArray (JsonArray)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|Json")
		static TArray<FString> Conv_JsonArraytToStringArray(FJsonStruct InFJsonStruct);
	/*尝试解析字符串获取Array str=["a",0,true....]*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToStringArray (JsonSourceStr)", BlueprintAutocast), Category = "Utilities|Json")
		static TArray<FString> Conv_JsonStrToArray(FString str,bool& isSuccess);

	/*将字符串数组 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (TArray<FString>)", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_TarrayStrToJsonArray(const TArray<FString>& In);
	/*将整数数组 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (TArray<int32>)", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_TarrayIntToJsonArray(const TArray<int32>& In);
	/*尝试解析字符串获取Array int=[1,2,3....]*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToIntArray (JsonSourceStr)", BlueprintAutocast), Category = "Utilities|Json")
		static TArray<int32> Conv_JsonStrToIntArray(FString str, bool& isSuccess);
	/*将浮点数组 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (TArray<float>)", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_TarrayFloatToJsonArray(const TArray<float>& In);
	/*将bool数组 转换为json array*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonArray (TArray<bool>)", BlueprintAutocast), Category = "Utilities|Json")
		static FJsonStruct Conv_TarrayBoolToJsonArray(const TArray<bool>& In);
};
