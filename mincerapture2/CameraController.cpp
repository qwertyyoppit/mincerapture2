#include "CameraController.h"
#include "Time.h"
#include "Game.h"

void CameraController::Update()
{
	_camera.Position.x += Velocity.x;
	_camera.Position.y += Velocity.y;
    Velocity -= Velocity * (Decelerate * Time::delta);
    _camera.Position.z += (Distance - _camera.Position.z) * (Speed * Time::delta);
}

void CameraController::StartDrag()
{
    Velocity *= 0;
}

void CameraController::Drag()
{
    auto previous = Blockheads.worldGen.CollideLayer(_camera.ScreenToWorldPoint(Blockheads.CursorPosition - Blockheads.CursorDelta));
    auto curpos = Blockheads.worldGen.CollideLayer(_camera.ScreenToWorldPoint(Blockheads.CursorPosition));
    Velocity = previous - curpos;
}

void CameraController::Move(bool forward)
{
    Distance += 2 * forward ? -1 : 1;
}
