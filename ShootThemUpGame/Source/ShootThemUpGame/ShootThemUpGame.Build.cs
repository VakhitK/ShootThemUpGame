// Shoot Them Up Game. All Rights Reserved.

using UnrealBuildTool;

public class ShootThemUpGame : ModuleRules
{
	public ShootThemUpGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{	"Core",
			"CoreUObject",
			"Engine",
			"GameplayTasks",
			"InputCore",
			"NavigationSystem",
			"Niagara",
			"PhysicsCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			"ShootThemUpGame/Public/AI",
			"ShootThemUpGame/Public/AI/Decorators",
			"ShootThemUpGame/Public/AI/EQS",
			"ShootThemUpGame/Public/AI/Services",
			"ShootThemUpGame/Public/AI/Tasks",
			"ShootThemUpGame/Public/Animations",
			"ShootThemUpGame/Public/Components",
			"ShootThemUpGame/Public/Dev",
			"ShootThemUpGame/Public/Pickups",
            "ShootThemUpGame/Public/Player",
			"ShootThemUpGame/Public/UI",
			"ShootThemUpGame/Public/Weapons",
			"ShootThemUpGame/Public/Weapons/Components"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
