// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
#define PrintToScreen(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(text));
#define PrintToScreen_1(text, val1) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), val1));
#define PrintToScreen_2(text, val1, val2) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), val1, val2));
#define PrintToScreen_3(text, val1, val2, val3) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), val1, val2, val3));
#define PrintToScreen_4(text, val1, val2, val3, val4) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), val1, val2, val3, val4));
#define PrintToScreen_5(text, val1, val2, val3, val4, val5) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), val1, val2, val3, val4, val5));

#define PrintToScreen_Color(text, color) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, TEXT(text));
#define PrintToScreen_1_color(text, val1, color) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT(text), val1));
#define PrintToScreen_2_color(text, val1, val2, color) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT(text), val1, val2));
#define PrintToScreen_3_color(text, val1, val2, val3, color) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT(text), val1, val2, val3));
#define PrintToScreen_4_color(text, val1, val2, val3, val4, color) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT(text), val1, val2, val3, val4));
#define PrintToScreen_5_color(text, val1, val2, val3, val4, val5, color) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, color, FString::Printf(TEXT(text), val1, val2, val3, val4, val5));
