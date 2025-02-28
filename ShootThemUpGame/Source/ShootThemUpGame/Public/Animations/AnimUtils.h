#pragma once

class AnimUtils
{
public:
    template <typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;

        for (const auto& NotifyEvent : Animation->Notifies)
        {
            if (auto&& AnimNotify = Cast<T>(NotifyEvent.Notify))
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};