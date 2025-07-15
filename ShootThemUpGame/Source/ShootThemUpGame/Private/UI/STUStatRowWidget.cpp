// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUStatRowWidget::SetPlayerName(const FText& Text) const
{
    if (!PlayerNameText) return;
    PlayerNameText->SetText(Text);
}

void USTUStatRowWidget::SetKills(const FText& Text) const
{
    if (!KillsText) return;
    KillsText->SetText(Text);
}

void USTUStatRowWidget::SetDeaths(const FText& Text) const
{
    if (!DeathsText) return;
    DeathsText->SetText(Text);
}

void USTUStatRowWidget::SetTeam(const FText& Text) const
{
    if (!TeamText) return;
    TeamText->SetText(Text);
}

void USTUStatRowWidget::SetPlayerIndicatorVisibility(bool Visible) const
{
    if (!PlayerIndicatorImage) return;
    PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}