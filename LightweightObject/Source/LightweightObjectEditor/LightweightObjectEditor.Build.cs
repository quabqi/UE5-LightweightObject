using UnrealBuildTool;

public class LightweightObjectEditor : ModuleRules
{
    public LightweightObjectEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "UnrealEd",
                "ClassViewer",
                "PropertyEditor",
                "StructUtilsEditor",
                "LightweightObject",
            }
        );
    }
}