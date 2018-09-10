#include "SmoothLightState.hpp"
#include "LedStrip.hpp"
#include "Util.hpp"

SmoothLightState::SmoothLightState(LedStrip &led_strip) : strip(led_strip)
{
    Initialize();
}

void SmoothLightState::Initialize()
{
}

void SmoothLightState::Update()
{
    for (int i = changeLength; i > 0; i++)
    {
        int ledToSet = activeLED - i;
        if (ledToSet < 0)
        {
            break;
        }
        byte colorToSet = (byte)(activeColor - (changeLength - i));
        strip.SetColor(colorByID(colorToSet), ledToSet);
    }
    for (int i = activeLED - 30; i >= 0; i--)
    {
        strip.SetColor(colorByID(activeColor), i);
    }
    activeLED++;
    activeLED %= strip.PixelCount();
}

CRGB SmoothLightState::colorByID(byte colorNumber)
{
    colorNumber = 255 - colorNumber;
    if (colorNumber < 85)
    {
        return CRGB(255 - colorNumber * 3, 0, colorNumber * 3);
    }
    if (colorNumber < 170)
    {
        colorNumber -= 85;
        return CRGB(0, colorNumber * 3, 255 - colorNumber * 3);
    }
    colorNumber -= 170;
    return CRGB(colorNumber * 3, 255 - colorNumber * 3, 0);
}
