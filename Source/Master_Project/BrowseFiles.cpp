// Fill out your copyright notice in the Description page of Project Settings.

#include "Master_Project.h"
#include "BrowseFiles.h"
#include "DDSLoader.h"

#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"


bool UBrowseFiles::getFilesandFolders(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
	if (RootFolderFullPath.Len() < 1) return false;

	FPaths::NormalizeDirectoryName(RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();

	if (!Ext.Contains(TEXT("*")))
	{
		if (Ext == "")
		{
			Ext = "*.*";
		}
		else
		{
			Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
		}
	}

	FString FinalPath = RootFolderFullPath + "/" + Ext;
	FileManager.FindFiles(Files, *FinalPath, true, true);
	return true;
}

bool UBrowseFiles::getFilesandFoldersRecursively(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
	if (RootFolderFullPath.Len() < 1) return false;

	FPaths::NormalizeDirectoryName(RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();

	if (!Ext.Contains(TEXT("*")))
	{
		if (Ext == "")
		{
			Ext = "*.*";
		}
		else
		{
			Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
		}
	}

	FileManager.FindFilesRecursive(Files, *RootFolderFullPath, *Ext, true, true);
	return true;
}

static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}

	return nullptr;
}

bool UBrowseFiles::ModifiedCaptureComponent2D_SaveImage(class USceneCaptureComponent2D* Target, const FString ImagePath, const FLinearColor ClearColour)
{
	UTextureRenderTarget2D *myTextureTarget = Target->TextureTarget;
	const uint32 height = myTextureTarget->SizeY;
	const uint32 width = myTextureTarget->SizeX;

	// Bad scene capture component! No render target! Stay! Stay! Ok, feed!... wait, where was I?
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "debug msg");
	UE_LOG(LogTemp, Warning, TEXT("testerino"));
	if ((Target == nullptr) || (myTextureTarget == nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("no target ot texturetarget"));
		return false;
	}

	FRenderTarget* RenderTarget = myTextureTarget->GameThread_GetRenderTargetResource();
	if (RenderTarget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("no rendertarget"));
		return false;
	}

	TArray<FColor> RawPixels;

	

	

	// Format not supported - use PF_B8G8R8A8.
	if (myTextureTarget->GetFormat() != PF_B8G8R8A8)
	{
		// TRACEWARN("Format not supported - use PF_B8G8R8A8.");
		UE_LOG(LogTemp, Warning, TEXT("foramt not supported..."));
		return false;
	}

	if (!RenderTarget->ReadPixels(RawPixels))
	{
		UE_LOG(LogTemp, Warning, TEXT("readpixels return false"));
		return false;
	}

	// Convert to FColor.
	FColor ClearFColour = ClearColour.ToFColor(false); // FIXME - want sRGB or not?

	for (auto& Pixel : RawPixels)
	{
		// Switch Red/Blue changes.
		const uint8 PR = Pixel.R;
		const uint8 PB = Pixel.B;
		Pixel.R = PB;
		Pixel.B = PR;

		// Set alpha based on RGB values of ClearColour.
		Pixel.A = ((Pixel.R == ClearFColour.R) && (Pixel.G == ClearFColour.G) && (Pixel.B == ClearFColour.B)) ? 0 : 255;
	}

	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);

	if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(&RawPixels[0], RawPixels.Num() * sizeof(FColor), width, height, ERGBFormat::RGBA, 8))
	{
		FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(), *ImagePath);
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("imagewrapper issue"));
	return false;
}

UTextureRenderTarget2D* UBrowseFiles::CreateRenderTarget2DTexture(int32 width, int32 height)
{
	UTextureRenderTarget2D *myRenderTarget = NewObject<UTextureRenderTarget2D>();

	myRenderTarget->InitCustomFormat(width, height, PF_B8G8R8A8, false);

	return myRenderTarget;
}

