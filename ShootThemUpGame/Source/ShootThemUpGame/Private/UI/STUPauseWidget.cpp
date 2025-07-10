// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

bool USTUPauseWidget::Initialize()
{
    const auto InitStatus = Super::Initialize();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }

    return InitStatus;
}

void USTUPauseWidget::OnClearPause()
{
    const auto World = GetWorld();
    if (!World || !World->GetAuthGameMode()) return;

    World->GetAuthGameMode()->ClearPause();
}