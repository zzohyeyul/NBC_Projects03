#include "NBCGameMode.h"
#include "NBCPawn.h"
#include "NBCPlayerController.h"

ANBCGameMode::ANBCGameMode()
{
	DefaultPawnClass = ANBCPawn::StaticClass();
	PlayerControllerClass = ANBCPlayerController::StaticClass();
}