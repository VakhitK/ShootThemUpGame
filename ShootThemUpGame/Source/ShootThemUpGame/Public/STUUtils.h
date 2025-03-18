#pragma once

class STUUtils
{
public:
    template <typename T>
    static T* GetComponent(const AActor* Player)
    {
        if (!Player) return nullptr;

        const auto Component = Player->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
};
