#include "UISubsystem.h"

UUISubsystem::UUISubsystem() {
	UIAsset = LoadObject<UUIDataAsset>(nullptr, TEXT("/Game/Luminaria/DataAssets/UIAsset"));
}
