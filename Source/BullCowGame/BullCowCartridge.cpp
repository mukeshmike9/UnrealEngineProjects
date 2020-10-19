// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    //Load Word List from File System
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/WordList.txt");
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);

    PrintLine(TEXT("Number of Words are: %i"), WordList.Num());
    WordList = GetValidWords(WordList);
    PrintLine(TEXT("Number of Valid Words are: %i"), WordList.Num());

    SetupGame(); //Setting game

    //Debug Line
     PrintLine(TEXT("The Hidden Word is %s and \nis %i characters long"), *sHiddenWord, sHiddenWord.Len());
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (bGameOver) // If game is over then do ClearScreen() and SetupGame()
    {
        ClearScreen();
        SetupGame();
        return;
    }
    // else Check Player Guess

    ProcessGuess(Input);
}

void UBullCowCartridge::SetupGame()
{
    //Welcome Player
    PrintLine(TEXT("Welcome to the Bull Cow Game...!!!"));
    sHiddenWord = WordList[FMath::RandRange(0, WordList.Num() - 1)]; //Set Hidden Word
    iRemainingLives = sHiddenWord.Len();                             //Set lives
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), sHiddenWord.Len());
    PrintLine(TEXT("You have %i chances"), iRemainingLives);
    //Prompt Player for guess
    PrintLine(TEXT("Type in your guess and\nPress Enter to continue"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    if (iRemainingLives == 0)
    {
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was %s"), *sHiddenWord);
    }

    PrintLine(TEXT("Press Enter to Play Again"));
}

void UBullCowCartridge::ProcessGuess(const FString Guess)
{
    if (Guess.Len() == 0)
    {
        PrintLine(TEXT("Enter a Guess"));
        return;
    }

    if (Guess == sHiddenWord)
    {
        PrintLine(TEXT("You have Won"));
        EndGame();
        return;
    }

    //Check if Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("There are repeating letters,\nHence this is not an Isogram\nGuess Again"));
        return;
    }

    //Prompt to guess Again
    //Check Right number of characters
    if (Guess.Len() != sHiddenWord.Len())
    {
        PrintLine(FString::Printf(TEXT("The Hidden Word is %i Characters long,\nPlease try again"), sHiddenWord.Len()));
        return;
    }

    //Decrement the lives
    //Check if lives remaining and inform user about remaining lives
    if (--iRemainingLives <= 0)
    {
        //If no lives left, Game Over and prompt user to play again
        PrintLine(TEXT("Game Over...!!!"));
        EndGame();
        return;
    }

    FBullCowCount Count = GetBullsAndCows(Guess);
    PrintLine(TEXT("You have got %i Bulls and %i Cows"), Count.Bulls, Count.Cows);

    PrintLine(TEXT("Try Again, You have lost a life\nYou have %i Tries remaining"), iRemainingLives);
}

bool UBullCowCartridge::IsIsogram(const FString Word) const
{
    //Loop through all charcter
    for (int32 i = 0; i < Word.Len(); i++)
    {
        //Internal loop through remaining char after external loop character
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            //Compare char, If char matches, return false
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullsAndCows(const FString &Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == sHiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 i = 0; i < sHiddenWord.Len(); i++)
        {
            if(Guess[GuessIndex] == sHiddenWord[i])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}