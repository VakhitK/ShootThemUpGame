// Shoot Them Up Game. All Rights Reserved.

#include "STUGameModeBase.h"

#include "AIController.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

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
        SetPlayerColor(Controller);
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

void ASTUGameModeBase::CreateTeamsInfo()
{
    UWorld* World = GetWorld();
    if (!World) return;

    int32 TeamID = 1;
    for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamId(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }

    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for TeamID: %i, set default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(const AController* Controller)
{
    if (!Controller) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    if (auto&& Character = Cast<ASTUBaseCharacter>(Controller->GetPawn()))
    {

        Character->SetPlayerColor(PlayerState->GetTeamColor());
    }
}