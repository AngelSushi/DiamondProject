#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"



class HTargetingVisProxy;

struct CUSTOMLUMINARIAMODULE_API HMecanismVisProxy : public HComponentVisProxy {
    DECLARE_HIT_PROXY();

    HMecanismVisProxy(const UActorComponent* InComponent): HComponentVisProxy(InComponent, HPP_Wireframe){}
};

struct CUSTOMLUMINARIAMODULE_API HMecanismProxy : public HMecanismVisProxy {

    DECLARE_HIT_PROXY();

public:
    HMecanismProxy(const UActorComponent* InComponent, int32 InTargetIndex);

    int32 TargetIndex;
};
