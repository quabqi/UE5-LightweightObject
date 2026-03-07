#include "LightweightObjectDetails.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "InstancedStructDetails.h"
#include "LightweightObject.h"
#include "PropertyBagDetails.h"

TSharedRef<IPropertyTypeCustomization> FLightweightObjectDetails::MakeInstance()
{
	return MakeShared<FLightweightObjectDetails>();
}

void FLightweightObjectDetails::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
                                                class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	StructProperty = StructPropertyHandle;
	ClassProperty = StructProperty->GetChildHandle(GET_MEMBER_NAME_CHECKED(FLightweightObject, Class));
	ClassProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FLightweightObjectDetails::OnClassChanged));
	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(250.f)
		.VAlign(VAlign_Center)
		[
			ClassProperty->CreatePropertyValueWidget()
		].IsEnabled(StructPropertyHandle->IsEditable())
		.ShouldAutoExpand(true);
}

void FLightweightObjectDetails::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
	class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	DataProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FLightweightObject, Data));
	PropUtils = StructCustomizationUtils.GetPropertyUtilities();
	FPropertyBagInstanceDataDetails::FConstructParams Params
	{
		.BagStructProperty = DataProperty,
		.PropUtils = PropUtils,
		.bAllowContainers = true,
		.ChildRowFeatures = EPropertyBagChildRowFeatures::Fixed
	};
	
	// Show the Value (FInstancedStruct) as child rows.
	const TSharedRef<FPropertyBagInstanceDataDetails> InstanceDetails = MakeShared<FPropertyBagInstanceDataDetails>(Params);
	StructBuilder.AddCustomBuilder(InstanceDetails);
}

void FLightweightObjectDetails::OnClassChanged() const
{
	TArray<FLightweightObject*> LightweightObjects;
	ClassProperty->AccessRawData( reinterpret_cast<TArray<void*>&>(LightweightObjects));
	for (FLightweightObject*& ObjectPtr : LightweightObjects)
	{
		if (ObjectPtr->Class == nullptr)
		{
			continue;
		}
		ULightweightObjectBase* DefaultObject = ObjectPtr->Class->GetDefaultObject<ULightweightObjectBase>();
		if (DefaultObject != nullptr)
		{
			DefaultObject->MigrateData(*ObjectPtr);
		}
	}
}
