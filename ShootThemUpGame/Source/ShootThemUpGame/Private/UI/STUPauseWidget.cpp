// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
}

void USTUPauseWidget::OnClearPause()
{
    const auto World = GetWorld();
    if (!World || !World->GetAuthGameMode()) return;

    World->GetAuthGameMode()->ClearPause();
}