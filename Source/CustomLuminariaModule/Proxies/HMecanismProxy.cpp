#include "HMecanismProxy.h"

HMecanismProxy::HMecanismProxy(const UActorComponent* InComponent, int32 InTargetIndex) : HTargetingVisProxy(InComponent), TargetIndex(InTargetIndex)
{
}

HMecanismProxy::~HMecanismProxy() {}
