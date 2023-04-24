// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(HUNT, Log, All);

#define HUNT_LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define HUNT_LOG_S(Verbosity) UE_LOG(HUNT, Verbosity, TEXT("%s"), *HUNT_LOG_CALLINFO)
#define HUNT_LOG(Verbosity, Format, ...) UE_LOG(HUNT, Verbosity, TEXT("%s %s"), *HUNT_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define HUNT_CHECK(Expr, ...) {if(!(Expr)) {HUNT_LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}