#include "plugin.h"

#include "CFont.h"

using namespace plugin;

bool isPressingShift = false;
bool isThrustOn = false;

short targetThrust = 0;
//short prevThrust = -1;

//bool canLerpThrust = false;

class Mod {
public:
	static float naive_lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static short GetThrust(CVehicle* veh) {
		uintptr_t addr = reinterpret_cast<uintptr_t>(veh) + 0x86C;
		return patch::GetShort(addr);
	}

	static void SetThrust(CVehicle* veh, short thrust) {
		uintptr_t addr = reinterpret_cast<uintptr_t>(veh) + 0x86C;
		return patch::SetShort(addr, thrust);
	}

	static void ProcessInput() {
		if (plugin::KeyPressed(VK_LSHIFT)) {

			if (isPressingShift) return;
			isPressingShift = true;

			isThrustOn = !isThrustOn;

			targetThrust = isThrustOn ? 5000 : 0;

			//canLerpThrust = true;
			//prevThrust = -1;
		}
		else {
			isPressingShift = false;
		}
	}

	static void UpdateThrust() {
		CVehicle* veh = FindPlayerVehicle(0, false);

		if (veh > 0) {
			short thrust = GetThrust(veh);
			//if (prevThrust == -1) prevThrust = thrust;

			/*
			if (canLerpThrust) {
				char text[256];
				sprintf(text, "%d thrust; %d prevThrust; %d targetThrust; %s canLerpThrust", thrust, prevThrust, targetThrust, canLerpThrust ? "true" : "false");
				//MessageBox(HWND_DESKTOP, text, "", MB_ICONERROR);
			}

			
			if (prevThrust != thrust) canLerpThrust = false;
			*/

			//if (canLerpThrust) {
			thrust = (short)round(naive_lerp((float)thrust, (float)targetThrust, 0.02f));

			if (abs(targetThrust - thrust) < 50) {
				thrust = targetThrust;
				//canLerpThrust = false;
			}
					
			SetThrust(veh, thrust);
				
			//}
		}
	}

	static void Draw() {
		CFont::SetOrientation(ALIGN_CENTER);
		CFont::SetColor(CRGBA(0, 255, 0, 255));
		CFont::SetDropShadowPosition(1);
		CFont::SetBackground(false, false);
		CFont::SetWrapx(500.0);
		CFont::SetScale(0.5, 1.0);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetProportional(true);

		CVehicle* veh = FindPlayerVehicle(0, false);

		char text[256];
		sprintf(text, "%d thrust; %d target", veh > 0 ? GetThrust(veh) : -1, targetThrust);
		CFont::PrintString(200, 400, text);
	}

    Mod() {
        Events::processScriptsEvent += []() {
			ProcessInput();
			UpdateThrust();
        };

		Events::drawingEvent += []() {
			//Draw();
		};
    }
} mod;
