#include "LightweightObjectEditor.h"

#include "LightweightObjectDetails.h"

#define LOCTEXT_NAMESPACE "FLightweightObjectEditorModule"

void FLightweightObjectEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("LightweightObject", 
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FLightweightObjectDetails::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FLightweightObjectEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout("LightweightObject");
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}
#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLightweightObjectEditorModule, LightweightObjectEditor)