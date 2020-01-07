// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Vaqueros : ModuleRules
{
	public Vaqueros(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // Add Json Modules
        PublicDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities", "UMG" });

        // Add Tasks and AI
        PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
