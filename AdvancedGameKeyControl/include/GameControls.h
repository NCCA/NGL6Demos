#ifndef GAME_CONTROLS_H__
#define GAME_CONTROLS_H__

enum GameControls
{
	kUp = 1 << 0,
	kDown = 1 << 1,
	kLeft = 1 << 2,
	kRight = 1 << 3,
	kRotate = 1 << 4,

	kUpLeft = kUp | kLeft,
	kUpRight = kUp | kRight,
	kDownLeft = kDown | kLeft,
	kDownRight = kDown | kRight,

	// nonsense controls
	kUpDown = kUp | kDown,
	kLeftRight = kRight | kLeft
};

// motion of a spaceship for a given key combo.
struct SpaceShipMotion
{
	float offsetX;
	float offsetY;
	float rotation;
};


// can be given to designers to tweak (loaded from resource possibly)
static const SpaceShipMotion g_motionTable[] =
{
	{ 0.0f, 0.0f, 0.0f }, // 0
	{ 0.0f, 1.0f, 0.0f }, // kUp
	{ 0.0f,-1.0f, 0.0f }, // kDown
	{ 0.0f, 0.0f, 0.0f }, // kUpDown (nonsense)

	{-1.0f, 0.0f, 0.0f }, // kLeft
	{-0.707f, 0.707f, 0.0f }, // kUpLeft
	{-0.707f,-0.707f, 0.0f }, // kDownLeft
	{-1.0f, 0.0f, 0.0f }, // kUpDown (nonsense) & kLeft

	{ 1.0f, 0.0f, 0.0f }, // kRight
	{ 0.707f, 0.707f, 0.0f }, // kUpRight
	{ 0.707f,-0.707f, 0.0f }, // kDownRight
	{ 1.0f, 0.0f, 0.0f }, // kUpDown (nonsense) & kRight

	{ 0.0f, 0.0f, 0.0f }, // kLeftRight (nonsense)
	{ 0.0f, 1.0f, 0.0f }, // kUp & kLeftRight (nonsense)
	{ 0.0f,-1.0f, 0.0f }, // kDown & kLeftRight (nonsense)
	{ 0.0f, 0.0f, 0.0f }, // kUpDown (nonsense) & kLeftRight (nonsense)

	// add rotation....
	{ 0.0f, 0.0f, 1.0f }, // 0
	{ 0.0f, 1.0f, 1.0f }, // kUp
	{ 0.0f,-1.0f, 1.0f }, // kDown
	{ 0.0f, 0.0f, 1.0f }, // kUpDown (nonsense)

	{-1.0f, 0.0f, 1.0f }, // kLeft
	{-0.707f, 0.707f, 1.0f }, // kUpLeft
	{-0.707f,-0.707f, 1.0f }, // kDownLeft
	{-1.0f, 0.0f, 1.0f }, // kUpDown (nonsense) & kLeft

	{ 1.0f, 0.0f, 1.0f }, // kRight
	{ 0.707f, 0.707f, 1.0f }, // kUpRight
	{ 0.707f,-0.707f, 1.0f }, // kDownRight
	{ 1.0f, 0.0f, 1.0f }, // kUpDown (nonsense) & kRight

	{ 0.0f, 0.0f, 1.0f }, // kLeftRight (nonsense)
	{ 0.0f, 1.0f, 1.0f }, // kUp & kLeftRight (nonsense)
	{ 0.0f,-1.0f, 1.0f }, // kDown & kLeftRight (nonsense)
	{ 0.0f, 0.0f, 1.0f }, // kUpDown (nonsense) & kLeftRight (nonsense)
};

const static float s_xExtents = 40;
const static float s_yExtents = 30;


#endif
