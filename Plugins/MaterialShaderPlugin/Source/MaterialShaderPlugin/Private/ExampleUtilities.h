// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Usage: DEFINE_PRIVATE_ACCESS(FMyClass, MyProperty) in global scope, then PrivateAccess::MyProperty(MyObject) from anywhere
#define DEFINE_PRIVATE_ACCESS(Class, Property) \
	namespace PrivateAccess \
	{ \
		template<typename> \
		struct TClass_ ## Property; \
		\
		template<> \
		struct TClass_ ## Property<Class> \
		{ \
			template<auto PropertyPtr> \
			struct TProperty_ ## Property \
			{ \
				friend auto& Property(Class& Object) \
				{ \
					return Object.*PropertyPtr; \
				} \
				friend auto& Property(const Class& Object) \
				{ \
					return Object.*PropertyPtr; \
				} \
			}; \
		}; \
		template struct TClass_ ## Property<Class>::TProperty_ ## Property<&Class::Property>; \
		\
		auto& Property(Class& Object); \
		auto& Property(const Class& Object); \
	}

struct FExampleUtilities
{
	static FRDGBufferRef FindBuffer(
		FRDGBuilder& GraphBuilder,
		const FString& Name);

	static FRDGTextureRef FindTexture(
		FRDGBuilder& GraphBuilder,
		const FString& Name);
};