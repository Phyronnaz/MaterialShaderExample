// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "ExampleUtilities.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphResources.h"

DEFINE_PRIVATE_ACCESS(FRDGBuilder, Buffers);
DEFINE_PRIVATE_ACCESS(FRDGBuilder, Textures);

FRDGBufferRef FExampleUtilities::FindBuffer(FRDGBuilder& GraphBuilder, const FString& Name)
{
	FRDGBufferRef Result = nullptr;

	FRDGBufferRegistry& Buffers = PrivateAccess::Buffers(GraphBuilder);
	Buffers.Enumerate([&](FRDGBuffer* Buffer)
	{
		if (FStringView(Buffer->Name) != Name)
		{
			return;
		}

		ensure(!Result);
		Result = Buffer;
	});

	return Result;
}

FRDGTextureRef FExampleUtilities::FindTexture(
	FRDGBuilder& GraphBuilder,
	const FString& Name)
{
	FRDGTextureRef Result = nullptr;

	FRDGTextureRegistry& Textures = PrivateAccess::Textures(GraphBuilder);
	Textures.Enumerate([&](FRDGTexture* Texture)
	{
		if (FStringView(Texture->Name) != Name)
		{
			return;
		}

		ensure(!Result);
		Result = Texture;
	});

	return Result;
}