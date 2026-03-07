#pragma once

#define UE_API LIGHTWEIGHTOBJECTEDITOR_API

class FLightweightObjectDetails : public IPropertyTypeCustomization
{
public:
	static UE_API TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	UE_API virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	UE_API virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	
private:
	void OnClassChanged() const;
	
	TSharedPtr<IPropertyHandle> StructProperty;
	TSharedPtr<IPropertyHandle> ClassProperty;
	TSharedPtr<IPropertyHandle> DataProperty;
	TSharedPtr<IPropertyUtilities> PropUtils;
};

#undef UE_API