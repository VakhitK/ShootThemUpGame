// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUPGAME_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASTUGameModeBase();

    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    void Killed(const AController* KillerController, const AController* VictimController) const;

    FGameData GetGameData() { return GameData; }
    int32 GetCurrentRound() const { return CurrentRound; }
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; }

    void RespawnRequest(AController* Controller);

    FOnMatchStateChangedSignature OnMatchStateChanged;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    void SpawnBots();
    void RespawnBots();
    void RespawnPlayer(AController* Controller);
    void StartRound();
    void GameTimerUpdate();
    void CreateTeamsInfo() const;
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    static void SetPlayerColor(const AController* Controller);
    void StartRespawn(const AController* Controller) const;
    void GameOver();
    void SetMatchState(ESTUMatchState State);
    void LogPlayerInfo() const;

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
};
