// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonHelper.h"
#include "StringTools.h"
FJsonStruct UJsonHelper::ParseJson(FString str)
{
	TSharedPtr<FJsonValue> JsonParsed;
	MyTools::ParseJson(JsonParsed, str);
	FJsonStruct out;
	out.value = JsonParsed;
	return out;
}

bool UJsonHelper::JsonIsValid(const FJsonStruct & In)
{
	if (!In.value||
		!In.value.IsValid()||
		In.value->IsNull())return false;
	return true;
}

FString UJsonHelper::ToString(FJsonStruct json)
{
	if(!json.value.IsValid())return FString();
	switch (json.value->Type) {
	case EJson::None:case EJson::Null:break;
	case EJson::String:case EJson::Number:case EJson::Boolean:
		return TEXT("\"")+json.key+TEXT("\":")+TryGetBaseValue(json);
		break;
	case EJson::Array:
		return 	MyTools::CreateJson(json.value->AsArray());
		break;
	case EJson::Object:
		return 	MyTools::CreateJson(json.value->AsObject());
		break;
	}
	return 	FString();
}

EJsonType UJsonHelper::JsonType(FJsonStruct json)
{
	switch (json.value->Type) {
	case EJson::None:return EJsonType::None;break;
	case EJson::Null:return EJsonType::Null;break;
	case EJson::String:return EJsonType::String;break;
	case EJson::Number:return EJsonType::Number;break;
	case EJson::Boolean:return EJsonType::Boolean;break;
	case EJson::Array:	return EJsonType::Array; break;
	case EJson::Object:	return EJsonType::Object; break;
	}
	return EJsonType::None;
}

FString UJsonHelper::TryGetBaseValue(FJsonStruct json)
{
	if(json.value&&json.value.IsValid())
		return MyTools::ParseJsonValueSingle(json.value);

	return FString();
}

TArray<FJsonStruct> UJsonHelper::GetAllValueFromJson(FJsonStruct json)
{
	TArray<FJsonStruct> out;
	if(!json.value.IsValid())	return out;
	if (json.value->Type == EJson::Object) {
		for (auto tem : json.value->AsObject()->Values) {
			FJsonStruct addTem;
			addTem.value = tem.Value;
			addTem.key = tem.Key;
			out.Add(addTem);
		}
	}
	else if (json.value->Type == EJson::Array) {
		for (auto tem : json.value->AsArray()) {
			FJsonStruct addTem;
			addTem.value = tem;
			out.Add(addTem);
		}
	}
	else out.Add(json);
	return out;
}

FJsonStruct UJsonHelper::FindJsonFromObject(FJsonStruct json, FString key,EJsonType type,bool& isFound)
{
	isFound = false;
	FJsonStruct out;
	TSharedPtr<FJsonValue> result;
	if (FindJsonFromObject_C(json.value, key,(EJson)type,result)) {
		isFound = true;
		out.key = key;
		out.value = result;
	}
	return out;
}

bool UJsonHelper::FindJsonFromObject_C(TSharedPtr<FJsonValue> json, FString key,EJson type,TSharedPtr<FJsonValue>& result)
{
	if (!json.IsValid() || (json->Type != EJson::Object&&json->Type != EJson::Array))	return false;
	if (json->Type == EJson::Array) {
		auto temArray = json->AsArray();
		for (auto tem : temArray) {
			if (FindJsonFromObject_C(tem, key,type, result))return true;
		}
		return false;
	}
	else {
		if (!json->AsObject()->HasField(key))return false;
		switch (type)
		{
		case EJson::None:case EJson::Null:return false; break;
		case EJson::String: {
			auto tem = json->AsObject()->GetField<EJson::String>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			else return false;
			break;
		}
		case EJson::Number: {
			auto tem = json->AsObject()->GetField<EJson::Number>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			else return false;
			break;
		}
		case EJson::Boolean: {
			auto tem = json->AsObject()->GetField<EJson::Boolean>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			else return false;
			break;
		}
		case EJson::Array: {
			auto tem = json->AsObject()->GetField<EJson::Array>(key);
			if (tem.IsValid()){
				result = tem;
				return true;
			}
			break;
		}
		case EJson::Object: {
			auto tem = json->AsObject()->GetField<EJson::Object>(key);
			if (tem.IsValid()) {
				result = tem;
				return true;
			}
			break;
		}
		}
		return false;
	}
}

FJsonStruct UJsonHelper::SetJsonKey(FJsonStruct json, FString key)
{
	json.key = key;
	return json;
}

void UJsonHelper::ChangeJsonArrayValue(UPARAM(ref)FJsonStruct& json, FJsonStruct from, FJsonStruct to, bool& isSuccess)
{
	isSuccess = false;
	if (!json.value.IsValid() || json.value->Type != EJson::Array)return ;
	if (!from.value.IsValid() || !to.value.IsValid())return ;
	auto arrays = json.value->AsArray();
	auto index = 0;
	for (auto tem : arrays) {
		FJsonStruct temStruct;
		temStruct.key = from.key;
		temStruct.value = tem;
		if (ToString(temStruct).Equals(ToString(from))) {
			arrays[index] = to.value;
			TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(arrays));
			json.value = Value;
			isSuccess = true;
			return ;
		}
		index++;
	}
}

void UJsonHelper::ChangeJsonObjectValue(FJsonStruct json, FString key, FJsonStruct value, bool& isSuccess)
{
	isSuccess = false;
	if (!json.value.IsValid() || json.value->Type != EJson::Object)return ;
	isSuccess = json.value->AsObject()->HasField(key);
	json.value->AsObject()->SetField(key, value.value);
}


void UJsonHelper::RemoveJsonArrayValueByValue(FJsonStruct json, FJsonStruct value, bool& isSuccess)
{
	isSuccess = false;
	if (!json.value.IsValid() || json.value->Type != EJson::Array)return ;
	if (!value.value.IsValid())return ;
	auto arrays = json.value->AsArray();
	auto index = 0;
	for (auto tem : arrays) {
		FJsonStruct temStruct;
		temStruct.key = value.key;
		temStruct.value = tem;
		if (ToString(temStruct).Equals(ToString(value))) {
			arrays.RemoveAt(index);
			TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(arrays));
			json.value = Value;
			isSuccess = true;
			return ;
		}
		index++;
	}
	return ;
}

void UJsonHelper::RemoveJsonObjectValueByKey(FJsonStruct json, FString key, bool& isSuccess)
{
	isSuccess = false;
	if (!json.value.IsValid() || json.value->Type != EJson::Object)return ;
	isSuccess=json.value->AsObject()->HasField(key);
	if(isSuccess)
		json.value->AsObject()->RemoveField(key);
	//return json;
}

void UJsonHelper::AddJsonArrayValue(UPARAM(ref)FJsonStruct & json, FJsonStruct value, bool & isSuccess)
{
	isSuccess = false;
	if (!json.value.IsValid() || json.value->Type != EJson::Array)return;
	if (!value.value.IsValid())return;
	auto temArray = json.value->AsArray();
	temArray.Add(value.value);
	TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(temArray));
	json.value = Value;
	isSuccess = true;
}
void UJsonHelper::AddJsonObjectValue(FJsonStruct json, FJsonStruct value, bool& isSuccess, bool isRepleace)
{
	isSuccess = false;
	if (!json.value.IsValid() || json.value->Type != EJson::Object)return ;
	if (!value.value.IsValid())return ;
	if (isRepleace) {
		json.value->AsObject()->SetField(value.key, value.value);
		isSuccess = true;
		return ;
	}
	else {
		if (json.value->AsObject()->HasField(value.key))return ;
		json.value->AsObject()->SetField(value.key, value.value);
		isSuccess = true;
		return ;
	}
}

FJsonStruct UJsonHelper::CreateJsonStringValue(FString key, FString value)
{
	FJsonStruct out;
	TSharedPtr<FJsonValueString> Value = MakeShareable(new FJsonValueString(value));
	out.key = key;
	out.value = Value;
	return out;
}

FJsonStruct UJsonHelper::CreateJsonNumberValue(FString key, float value)
{
	FJsonStruct out;
	TSharedPtr<FJsonValueNumber> Value = MakeShareable(new FJsonValueNumber(value));
	out.key = key;
	out.value = Value;
	return out;
}

FJsonStruct UJsonHelper::CreateJsonBoolValue(FString key, bool value)
{
	FJsonStruct out;
	TSharedPtr<FJsonValueBoolean> Value = MakeShareable(new FJsonValueBoolean(value));
	out.key = key;
	out.value = Value;
	return out;
}
FJsonStruct UJsonHelper::CreateJsonObject()
{
	TArray<FJsonStruct> tem;
	return CreateJsonObjectByValue(tem);
}
FJsonStruct UJsonHelper::CreateJsonObjectByValue(TArray<FJsonStruct> arrays)
{
	FJsonStruct out;
	TSharedPtr<FJsonObject> temObj = MakeShareable(new FJsonObject);
	TSharedPtr < FJsonValueObject > Value = MakeShareable(new FJsonValueObject(temObj));
	for (auto tem : arrays) {
		if (tem.value.IsValid())CreateJsonObjectValue_C(temObj, tem);
	}
	out.value = Value;
	return out;
}


void UJsonHelper::CreateJsonObjectValue_C(TSharedPtr<FJsonObject> obj, FJsonStruct addItem)
{
	switch (addItem.value->Type)
	{
	case EJson::None:case EJson::Null:break;
	case EJson::String:case EJson::Number:case EJson::Boolean:case EJson::Array:case EJson::Object:
		obj->SetField(addItem.key, addItem.value);
		break;
	}
}

FJsonStruct UJsonHelper::CreateJsonArray()
{
	TArray<FJsonStruct> tem;
	return CreateJsonArrayByValue(tem);
}

FJsonStruct UJsonHelper::CreateJsonArrayByValue(TArray<FJsonStruct> arrays)
{
	FJsonStruct out;
	TArray<TSharedPtr<FJsonValue>> temArray;
	for (auto tem : arrays)
		if (tem.value.IsValid())CreateJsonArrayValue_C(temArray, tem);
	TSharedPtr < FJsonValueArray > Value = MakeShareable(new FJsonValueArray(temArray));
	out.value = Value;
	return out;
}

void UJsonHelper::CreateJsonArrayValue_C(TArray<TSharedPtr<FJsonValue>>& ary, FJsonStruct addItem)
{
	switch (addItem.value->Type)
	{
	case EJson::None:case EJson::Null:break;
	case EJson::String:case EJson::Number:case EJson::Boolean:case EJson::Object:case EJson::Array:
		ary.Add(addItem.value);
		break;
	}
}

FString UJsonHelper::JsonObjectToKeyValuePairs(FJsonStruct json)
{
	FString out;
	if (!json.value.IsValid() || json.value->Type != EJson::Object)return out;
	auto object=json.value->AsObject();
	for (auto tem : object->Values) {
		out+=tem.Key+ TEXT("=")+MyTools::ParseJsonValueSingle(tem.Value)+TEXT("&");
	}
	out.RemoveFromEnd(TEXT("&"));
	return out.Replace(TEXT("\r\n\t"), TEXT("")).Replace(TEXT("\r\n"), TEXT(""));
}

bool UJsonHelper::Conv_StringToBool(const FString & InString)
{
	return InString.Equals(TEXT("true"))?true:false;
}


FJsonStruct UJsonHelper::Conv_VectorToJsonObject(FVector InFVector, FString Xkey, FString Ykey, FString Zkey)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(Xkey, InFVector.X));
	tem.Add(CreateJsonNumberValue(Ykey, InFVector.Y));
	tem.Add(CreateJsonNumberValue(Zkey, InFVector.Z));
	return CreateJsonObjectByValue(tem);
}

FJsonStruct UJsonHelper::Conv_VectorToJsonArray(FVector InFVector)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(TEXT(""), InFVector.X));
	tem.Add(CreateJsonNumberValue(TEXT(""), InFVector.Y));
	tem.Add(CreateJsonNumberValue(TEXT(""), InFVector.Z));
	return CreateJsonArrayByValue(tem);
}

FVector UJsonHelper::Conv_JsonObjecttToVector(FJsonStruct InFJsonStruct, FString Xkey, FString Ykey, FString Zkey)
{
	if (!InFJsonStruct.value.IsValid() || InFJsonStruct.value->Type != EJson::Object)return FVector();
	auto object=InFJsonStruct.value->AsObject();
	float X, Y, Z;
	X = Y = Z = 0;
	if (object->HasTypedField<EJson::Number>(Xkey)) {
		X = object->GetNumberField(Xkey);
	}
	if (object->HasTypedField<EJson::Number>(Ykey)) {
		Y = object->GetNumberField(Ykey);
	}
	if (object->HasTypedField<EJson::Number>(Zkey)) {
		Z = object->GetNumberField(Zkey);
	}
	return FVector(X,Y,Z);
}

FVector UJsonHelper::Conv_JsonArraytToVector(FJsonStruct InFJsonStruct)
{
	if (!InFJsonStruct.value.IsValid() || InFJsonStruct.value->Type != EJson::Array)return FVector();
	auto Array = InFJsonStruct.value->AsArray();
	float X, Y, Z;
	X = Y = Z = 0;
	auto index = 0;
	for (auto tem : Array) {
		if (tem->Type == EJson::Number) {
			switch (index)
			{
			case 0:X = tem->AsNumber();
				break;
			case 1:Y = tem->AsNumber();
				break;
			case 2:Z = tem->AsNumber();
				break;
			}
			index++;
			if (index >= 3)break;
		}
	}

	return FVector(X, Y, Z);
}

FJsonStruct UJsonHelper::Conv_RotatorToJsonObject(FRotator InFRotator, FString RollKey, FString PitchKey, FString YawKey)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(RollKey, InFRotator.Roll));
	tem.Add(CreateJsonNumberValue(PitchKey, InFRotator.Pitch));
	tem.Add(CreateJsonNumberValue(YawKey, InFRotator.Yaw));
	return CreateJsonObjectByValue(tem);
}

FJsonStruct UJsonHelper::Conv_RotatorToJsonArray(FRotator InFRotator)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(TEXT(""), InFRotator.Roll));
	tem.Add(CreateJsonNumberValue(TEXT(""), InFRotator.Pitch));
	tem.Add(CreateJsonNumberValue(TEXT(""), InFRotator.Yaw));
	return CreateJsonArrayByValue(tem);
}

FJsonStruct UJsonHelper::Conv_QuatToJsonObject(FQuat In, FString xKey, FString yKey, FString zKey, FString wKey)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(xKey, In.X));
	tem.Add(CreateJsonNumberValue(yKey, In.Y));
	tem.Add(CreateJsonNumberValue(zKey, In.Z));
	tem.Add(CreateJsonNumberValue(wKey, In.W));
	return CreateJsonObjectByValue(tem);
}

FJsonStruct UJsonHelper::Conv_QuatToJsonArray(FQuat In)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(TEXT(""), In.X));
	tem.Add(CreateJsonNumberValue(TEXT(""), In.Y));
	tem.Add(CreateJsonNumberValue(TEXT(""), In.Z));
	tem.Add(CreateJsonNumberValue(TEXT(""), In.W));
	return CreateJsonArrayByValue(tem);
}

FRotator UJsonHelper::Conv_JsonObjecttToRotator(FJsonStruct InFJsonStruct, FString Xkey, FString Ykey, FString Zkey)
{
	if (!InFJsonStruct.value.IsValid() || InFJsonStruct.value->Type != EJson::Object)return FRotator();
	auto object = InFJsonStruct.value->AsObject();
	float X, Y, Z;
	X = Y = Z = 0;
	if (object->HasTypedField<EJson::Number>(Xkey)) {
		X = object->GetNumberField(Xkey);
	}
	if (object->HasTypedField<EJson::Number>(Ykey)) {
		Y = object->GetNumberField(Ykey);
	}
	if (object->HasTypedField<EJson::Number>(Zkey)) {
		Z = object->GetNumberField(Zkey);
	}
	//Pitch, Yaw,Roll 
	// y,z ,x
	return FRotator(Y,Z,X);
}

FRotator UJsonHelper::Conv_JsonArraytToRotator(FJsonStruct InFJsonStruct)
{
	if (!InFJsonStruct.value.IsValid() || InFJsonStruct.value->Type != EJson::Array)return FRotator();
	auto Array = InFJsonStruct.value->AsArray();
	float Roll, Pitch, Yaw;
	Roll = Pitch = Yaw = 0;
	auto index = 0;
	for (auto tem : Array) {
		if (tem->Type == EJson::Number) {
			switch (index)
			{
			case 0:Roll = tem->AsNumber();
				break;
			case 1:Pitch = tem->AsNumber();
				break;
			case 2:Yaw = tem->AsNumber();
				break;
			}
			index++;
			if (index >= 3)break;
		}
	}

	return FRotator(Pitch, Yaw,Roll);
}

FJsonStruct UJsonHelper::Conv_Vector2DToJsonObject(FVector2D InFVector2D, FString Xkey, FString Ykey)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(Xkey, InFVector2D.X));
	tem.Add(CreateJsonNumberValue(Ykey, InFVector2D.Y));
	return CreateJsonObjectByValue(tem);
}

FJsonStruct UJsonHelper::Conv_Vector2DToJsonArray(FVector2D InFVector2D)
{
	TArray<FJsonStruct> tem;
	tem.Add(CreateJsonNumberValue(TEXT(""), InFVector2D.X));
	tem.Add(CreateJsonNumberValue(TEXT(""), InFVector2D.Y));
	return CreateJsonArrayByValue(tem);
}

FVector2D UJsonHelper::Conv_JsonObjecttToVector2D(FJsonStruct InFJsonStruct, FString Xkey, FString Ykey)
{
	if (!InFJsonStruct.value.IsValid() || InFJsonStruct.value->Type != EJson::Object)return FVector2D();
	auto object = InFJsonStruct.value->AsObject();
	float X, Y;
	X = Y = 0;
	if (object->HasTypedField<EJson::Number>(Xkey)) {
		X = object->GetNumberField(Xkey);
	}
	if (object->HasTypedField<EJson::Number>(Ykey)) {
		Y = object->GetNumberField(Ykey);
	}
	return FVector2D(X, Y);
}

FVector2D UJsonHelper::Conv_JsonArraytToVector2D(FJsonStruct InFJsonStruct)
{
	if (!InFJsonStruct.value.IsValid() || InFJsonStruct.value->Type != EJson::Array)return FVector2D();
	auto Array = InFJsonStruct.value->AsArray();
	float X, Y;
	X = Y = 0;
	auto index = 0;
	for (auto tem : Array) {
		if (tem->Type == EJson::Number) {
			switch (index)
			{
			case 0:X = tem->AsNumber();
				break;
			case 1:Y = tem->AsNumber();
				break;
			}
			index++;
			if (index >= 2)break;
		}
	}
	return FVector2D(X, Y);
}

TArray<FString> UJsonHelper::Conv_JsonArraytToStringArray(FJsonStruct InFJsonStruct)
{
	if (!InFJsonStruct.value || !InFJsonStruct.value.IsValid())return TArray<FString>();
	if (InFJsonStruct.value->Type != EJson::Array)return TArray<FString>();

	TArray<FString> Out;
	auto tem2 = GetAllValueFromJson(InFJsonStruct);
	for (auto tem3 : tem2)
	{
		Out.Add(TryGetBaseValue(tem3));
	}
	return Out;
}

TArray<FString> UJsonHelper::Conv_JsonStrToArray(FString str, bool & isSuccess)
{
	isSuccess = false;
	auto tem=ParseJson(str);
	if(!tem.value||!tem.value.IsValid())return TArray<FString>();

	if(tem.value->Type!=EJson::Array)return TArray<FString>();

	TArray<FString> Out;
	isSuccess = true;
	auto tem2= GetAllValueFromJson(tem);
	for (auto tem3 : tem2)
	{
		Out.Add(TryGetBaseValue(tem3));
	}
	return Out;
}

FJsonStruct UJsonHelper::Conv_TarrayStrToJsonArray(const TArray<FString>& In)
{
	TArray<FJsonStruct> tem;
	for (auto tem2 : In)
	{
		tem.Add(CreateJsonStringValue("", tem2));
	}
	return CreateJsonArrayByValue(tem);
}

FJsonStruct UJsonHelper::Conv_TarrayIntToJsonArray(const TArray<int32>& In)
{
	TArray<FJsonStruct> tem;
	for (auto tem2 : In)
	{
		tem.Add(CreateJsonNumberValue("", tem2));
	}
	return CreateJsonArrayByValue(tem);
}

TArray<int32> UJsonHelper::Conv_JsonStrToIntArray(FString str, bool & isSuccess)
{
	isSuccess = false;
	auto tem = ParseJson(str);
	if (!tem.value || !tem.value.IsValid())return TArray<int32>();

	if (tem.value->Type != EJson::Array)return TArray<int32>();

	TArray<int32> Out;
	isSuccess = true;
	auto tem2 = GetAllValueFromJson(tem);
	for (auto tem3 : tem2)
	{
		if (tem3.value->Type == EJson::Number)
		{
			auto tem4=tem3.value->AsNumber();
			Out.Add(tem4);
		}
	}
	return Out;
}

FJsonStruct UJsonHelper::Conv_TarrayFloatToJsonArray(const TArray<float>& In)
{
	TArray<FJsonStruct> tem;
	for (auto tem2 : In)
	{
		tem.Add(CreateJsonNumberValue("", tem2));
	}
	return CreateJsonArrayByValue(tem);
}

FJsonStruct UJsonHelper::Conv_TarrayBoolToJsonArray(const TArray<bool>& In)
{
	TArray<FJsonStruct> tem;
	for (auto tem2 : In)
	{
		tem.Add(CreateJsonBoolValue("", tem2));
	}
	return CreateJsonArrayByValue(tem);
}
