// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTTHEMUPGAME_API USTUStatRowWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerName(const FText& Text) const;
    void SetKills(const FText& Text) const;
    void SetDeaths(const FText& Text) const;
    void SetTeam(const FText& Text) const;
    void SetPlayerIndicatorVisibility(bool Visible) const;

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TeamText;

    UPROPERTY(meta = (BindWidget))
    UImage* PlayerIndicatorImage;
};