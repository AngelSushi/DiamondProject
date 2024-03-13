#include "HMecanismProxy.h"

IMPLEMENT_HIT_PROXY(HMecanismVisProxy,HComponentVisProxy)
IMPLEMENT_HIT_PROXY(HMecanismProxy,HMecanismVisProxy)

HMecanismProxy::HMecanismProxy(const UActorComponent* InComponent, int32 InTargetIndex) : HMecanismVisProxy(InComponent), TargetIndex(InTargetIndex)
{
}

