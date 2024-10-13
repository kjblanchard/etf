#include <SupergoonEngine/input/joystick.h>
#include <SupergoonEngine/input/keyboard.h>
namespace Supergoon {
enum class KeyboardKeys {
	Key_UNKNOWN = 0,
	Key_A = 4,
	Key_B = 5,
	Key_C = 6,
	Key_D = 7,
	Key_E = 8,
	Key_F = 9,
	Key_G = 10,
	Key_H = 11,
	Key_I = 12,
	Key_J = 13,
	Key_K = 14,
	Key_L = 15,
	Key_M = 16,
	Key_N = 17,
	Key_O = 18,
	Key_P = 19,
	Key_Q = 20,
	Key_R = 21,
	Key_S = 22,
	Key_T = 23,
	Key_U = 24,
	Key_V = 25,
	Key_W = 26,
	Key_X = 27,
	Key_Y = 28,
	Key_Z = 29,
	Key_1 = 30,
	Key_2 = 31,
	Key_3 = 32,
	Key_4 = 33,
	Key_5 = 34,
	Key_6 = 35,
	Key_7 = 36,
	Key_8 = 37,
	Key_9 = 38,
	Key_0 = 39,
	Key_RETURN = 40,
	Key_ESCAPE = 41,
	Key_BACKSPACE = 42,
	Key_TAB = 43,
	Key_SPACE = 44,
	Key_LSHIFT = 225
};

enum class GameControllerButton {
	GameControllerButtonA,
	GameControllerButtonB,
	GameControllerButtonX,
	GameControllerButtonY,
	GameControllerButtonBACK,
	GameControllerButtonGUIDE,
	GameControllerButtonSTART,
	GameControllerButtonLEFTSTICK,
	GameControllerButtonRIGHTSTICK,
	GameControllerButtonLEFTSHOULDER,
	GameControllerButtonRIGHTSHOULDER,
	GameControllerButtonDPAD_UP,
	GameControllerButtonDPAD_DOWN,
	GameControllerButtonDPAD_LEFT,
	GameControllerButtonDPAD_RIGHT,
	GameControllerButtonMISC1,	  /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
	GameControllerButtonPADDLE1,  /* Xbox Elite paddle P1 (upper left, facing the back) */
	GameControllerButtonPADDLE2,  /* Xbox Elite paddle P3 (upper right, facing the back) */
	GameControllerButtonPADDLE3,  /* Xbox Elite paddle P2 (lower left, facing the back) */
	GameControllerButtonPADDLE4,  /* Xbox Elite paddle P4 (lower right, facing the back) */
	GameControllerButtonTOUCHPAD, /* PS4/PS5 touchpad button */
	GameControllerButtonMAX
};

inline static bool KeyJustReleased(KeyboardKeys key) { return geKeyJustReleased((int)key); }
inline static bool KeyJustReleased(GameControllerButton key, int padNum) { return geGamepadButtonJustReleased((int)key, padNum); }
inline static bool KeyJustReleased(KeyboardKeys key, GameControllerButton button, int padNum) { return geKeyJustReleased((int)key) || geGamepadButtonJustReleased((int)button, padNum); }
inline static bool KeyJustPressed(KeyboardKeys key) { return geKeyJustPressed((int)key); }
inline static bool KeyJustPressed(GameControllerButton key, int padNum) { return geGamepadButtonJustPressed((int)key, padNum); }
inline static bool KeyJustPressed(KeyboardKeys key, GameControllerButton button, int padNum) { return geKeyJustPressed((int)key) || geGamepadButtonJustPressed((int)button, padNum); }
inline static bool KeyHeldDown(KeyboardKeys key) { return geKeyHeldDown((int)key); }
inline static bool KeyHeldDown(GameControllerButton key, int padNum) { return geGamepadButtonHeldDown((int)key, padNum); }
inline static bool KeyHeldDown(KeyboardKeys key, GameControllerButton button, int padNum) { return geKeyHeldDown(int(key)) || geGamepadButtonHeldDown((int)button, padNum); }
inline static bool KeyDown(KeyboardKeys key) { return geKeyHeldDown((int)key) || geKeyJustPressed((int)key); }
inline static bool KeyDown(GameControllerButton key, int padNum) { return geGamepadButtonJustPressed((int)key, padNum) || geGamepadButtonHeldDown((int)key, padNum); }
inline static bool KeyDown(KeyboardKeys key, GameControllerButton button, int padNum) { return KeyDown(key) || KeyDown(button, padNum); }

}  // namespace Supergoon
