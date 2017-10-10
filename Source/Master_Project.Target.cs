// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Master_ProjectTarget : TargetRules
{
	public Master_ProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        bUsesSteam = true;
        ExtraModuleNames.AddRange(new string[] { "Master_Project" });
    }
}
