// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshMaterialShader.h"

class FPrimitiveUniformShaderParameters;

BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FExampleMaterialShaderParameters, )
	SHADER_PARAMETER(uint32, ShadingBinToReplace)
	SHADER_PARAMETER_SCALAR_ARRAY(uint32, MaterialIndexToShadingBin, [16])
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D<UlongType>, VisBuffer64)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<uint>, ShadingMask)
END_GLOBAL_SHADER_PARAMETER_STRUCT()

class FExampleMaterialShader : public FMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FExampleMaterialShader, Material);

	FExampleMaterialShader() = default;
	explicit FExampleMaterialShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCompilePermutation(const FMaterialShaderPermutationParameters& Parameters);

	static void ModifyCompilationEnvironment(
		const FMaterialShaderPermutationParameters& Parameters,
		FShaderCompilerEnvironment& OutEnvironment);
};