// Fill out your copyright notice in the Description page of Project Settings.


#include "LightweightObject.h"

FLightweightObject ULightweightObjectBase::CreateLightweightObject() const
{
	FLightweightObject Object;
	Object.Class = GetClass();
	Object.Data = DefaultData;
	return Object;
}

bool ULightweightObjectBase::PushLightweightObject(const FLightweightObject& InLightweightObject)
{
	if (InLightweightObject.Class != GetClass())
	{
		return false;
	}
	if (InLightweightObject.Data.GetPropertyBagStruct() != DefaultData.GetPropertyBagStruct())
	{
		return false;
	}
	const UPropertyBag* PropertyBag = DefaultData.GetPropertyBagStruct();
	const void* LightweightObjectMemory = InLightweightObject.Data.GetValue().GetMemory();
	for (const FPropertyBagPropertyDesc& SourceDesc : PropertyBag->GetPropertyDescs())
	{
		const FProperty* SourceProperty = SourceDesc.CachedProperty;
		if (SourceProperty == nullptr)
		{
			return false;
		}
		const FName PropertyName = SourceProperty->GetFName();
		const FProperty* DestProperty = GetClass()->FindPropertyByName(PropertyName);
		if (DestProperty == nullptr)
		{
			return false;
		}

		void* DestAddress = DestProperty->ContainerPtrToValuePtr<void>(this);
		const void* SourceAddress = SourceProperty->ContainerPtrToValuePtr<void>(LightweightObjectMemory);
		DestProperty->CopyCompleteValue(DestAddress, SourceAddress);
	}
	return true;
}

bool ULightweightObjectBase::PopLightweightObject(FLightweightObject& OutLightweightObject)
{
	if (OutLightweightObject.Class != GetClass())
	{
		return false;
	}
	if (OutLightweightObject.Data.GetPropertyBagStruct() != DefaultData.GetPropertyBagStruct())
	{
		return false;
	}
	const UPropertyBag* PropertyBag = DefaultData.GetPropertyBagStruct();
	void* LightweightObjectMemory = OutLightweightObject.Data.GetMutableValue().GetMemory();
	for (const FPropertyBagPropertyDesc& DestDesc : PropertyBag->GetPropertyDescs())
	{
		const FProperty* DestProperty = DestDesc.CachedProperty;
		if (DestProperty == nullptr)
		{
			return false;
		}
		const FName PropertyName = DestProperty->GetFName();
		const FProperty* SourceProperty = GetClass()->FindPropertyByName(PropertyName);
		if (SourceProperty == nullptr)
		{
			return false;
		}

		void* DestAddress = DestProperty->ContainerPtrToValuePtr<void>(LightweightObjectMemory);
		const void* SourceAddress = SourceProperty->ContainerPtrToValuePtr<void>(this);
		DestProperty->CopyCompleteValue(DestAddress, SourceAddress);
	}
	return true;
}

void ULightweightObjectBase::ConvertToULightweightObject(const FLightweightObject& InObject, int32& OutObject)
{
	// We should never hit this! stubs to avoid NoExport on the class.
	checkNoEntry();
}

bool ULightweightObjectBase::ConvertToFLightweightObject(FLightweightObject& OutLightweightObject)
{
	OutLightweightObject.Class = GetClass();
	OutLightweightObject.Data = DefaultData;
	return PopLightweightObject(OutLightweightObject);
}

DEFINE_FUNCTION(ULightweightObjectBase::execConvertToULightweightObject)
{
	P_GET_STRUCT_REF(FLightweightObject, InObject);
	
	// Read wildcard Value input.
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FObjectProperty>(nullptr);
	
	FObjectProperty* ReturnProp = CastField<FObjectProperty>(Stack.MostRecentProperty);
	void* ReturnValueAddr = Stack.MostRecentPropertyAddress;
	
	P_FINISH;
	
	if (ReturnProp != nullptr && ReturnValueAddr != nullptr)
	{
		UClass* ExpectedClass = ReturnProp->PropertyClass;
		if (ExpectedClass != nullptr && ExpectedClass == InObject.Class)
		{
			P_NATIVE_BEGIN;
			ULightweightObjectBase* ResultObject = ExpectedClass->GetDefaultObject<ULightweightObjectBase>();
			if (ResultObject->PushLightweightObject(InObject))
			{
				ReturnProp->SetObjectPropertyValue(ReturnValueAddr, ResultObject);
			}
			else
			{
				ReturnProp->SetObjectPropertyValue(ReturnValueAddr, nullptr);
			}
			P_NATIVE_END;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ConvertFObjectToUObject: Expected class is not InObject.Class."));
			ReturnProp->SetObjectPropertyValue(ReturnValueAddr, nullptr);	
		}
	}
}

void ULightweightObjectBase::PostLoad()
{
	Super::PostLoad();
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		TArray<FPropertyBagPropertyDesc> BagProperties;
		for (TFieldIterator<FProperty> It(GetClass()); It; ++It)
		{
			const FProperty* Property = *It;
			const UClass* PropertyOwnerClass = Property->GetOwnerClass();
			if (PropertyOwnerClass == StaticClass() || PropertyOwnerClass == UObject::StaticClass())
			{
				continue;
			}
			const FName PropertyName = Property->GetFName();
			BagProperties.Add(FPropertyBagPropertyDesc(PropertyName, Property));
		}
		DefaultData.AddProperties(BagProperties);
		
		for (TFieldIterator<FProperty> It(GetClass()); It; ++It)
		{
			const FProperty* Property = *It;
			const UClass* PropertyOwnerClass = Property->GetOwnerClass();
			if (PropertyOwnerClass == StaticClass() || PropertyOwnerClass == UObject::StaticClass())
			{
				continue;
			}
			const FName PropertyName = Property->GetFName();
			DefaultData.SetValue(PropertyName, Property, this);
		}
	}
}

void ULightweightObjectBase::MigrateData(FLightweightObject& InLightweightObject) const
{
	if (InLightweightObject.Data.IsValid())
	{
		if (InLightweightObject.Data.GetPropertyBagStruct() != DefaultData.GetPropertyBagStruct())
		{
			InLightweightObject.Data.MigrateToNewBagInstance(DefaultData);
		}
	}
	else
	{
		InLightweightObject.Data = DefaultData;
	}
}
