// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    PrintLine(TEXT("Welcome to the Bull Cow Game...!!!"));
    PrintLine(TEXT("Guess the 4 letter word!")); // Magic number Remove
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    FString HiddenWord = TEXT("cake"); //Move outside this function
    
    if (Input == HiddenWord)    
    {
        PrintLine(TEXT("You have Won...!!!"));
    }
    else
    {
        PrintLine(TEXT("You have Lost...!!!"));
    }
}