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
    for (int i = 0; i < strip.PixelCount(); i++)
    {
        if (i < activeLED)
        {
            strip.SetColor(colorByID((byte)(activeColor + changeLength)), i);
        }
        else if (i >= activeLED && i < activeLED + 30)
        {
            int numberInTransition = i - activeLED;
            numberInTransition = changeLength - numberInTransition;
            strip.SetColor(colorByID((byte)(activeColor + numberInTransition)), i);
        }
    }

    activeLED++;
    if (activeLED >= strip.PixelCount())
    {
        activeLED = 0;
        activeColor += changeLength;
    }
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
