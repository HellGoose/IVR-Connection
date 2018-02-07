// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImageUtils.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrowseFiles.generated.h"



/**
 * 
 */
UCLASS()
class MASTER_PROJECT_API UBrowseFiles : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		/** Obtain all files and folders in a provided directory, with optional extension filter. All files are returned if Ext is left blank. Returns false if operation could not occur. Created by rama, modified*/
		UFUNCTION(BlueprintPure, Category = "CustomNodes|Browse Folders")
		static bool getFilesandFolders(TArray<FString>& Files, FString RootFolderFullPath, FString Ext);

		/** Obtain all files and folders in a provided root directory, including all subdirectories, with optional extension filter. All files are returned if Ext is left blank. The full file path is returned because the file could be in any subdirectory. Returns false if operation could not occur. Created by rama, modified */
		UFUNCTION(BlueprintPure, Category = "CustomNodes|Browse Folders")
		static bool getFilesandFoldersRecursively(TArray<FString>& Files, FString RootFolderFullPath, FString Ext);
	 
		/** Make sure to include the appropriate image extension in your file path! Recommended: .bmp, .jpg, .png. Contributed by Community Member Kris!, modified by me */
		UFUNCTION(Category = "CustomNodes|SceneCapture", BlueprintCallable)
		static bool ModifiedCaptureComponent2D_SaveImage(class USceneCaptureComponent2D* Target, const FString ImagePath, const FLinearColor ClearColour);


		UFUNCTION(BlueprintPure, Category = "CustomNodes|SceneCapture")
		static UTextureRenderTarget2D* CreateRenderTarget2DTexture(int32 width, int32 height);
};
