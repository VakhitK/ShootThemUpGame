// Shoot Them Up Game. All Rights Reserved.

#include "STUGameModeBase.h"

#include "AIController.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();

    CurrentRound = 1;
    StartRound();
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::SpawnBots()
{
    UWorld* World = GetWorld();
    if (!World || !AIControllerClass) return;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const int32 PlayersNum = GameData.PlayersNum - 1;
    for (int32 i = 0; i < PlayersNum; ++i)
    {
        if (auto&& AIController = World->SpawnActor<AAIController>(AIControllerClass, SpawnInfo))
        {
            RestartPlayer(AIController);
        }
    }
}

void ASTUGameModeBase::RespawnBots()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
    {

        RespawnPlayer(It->Get());
    }
}

void ASTUGameModeBase::RespawnPlayer(AController* Controller)
{
    if (Controller)
    {
        if (auto&& Pawn = Controller->GetPawn())
        {
            Pawn->Reset();
        }

        RestartPlayer(Controller);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {

            ++CurrentRound;
            RespawnBots();
            StartRound();
        }
        else
        {
            UE_LOG(LogSTUGameModeBase, Display, TEXT("Game over!"));
        }
    }
}