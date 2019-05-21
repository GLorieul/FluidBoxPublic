
#include <iostream>
#include <gtest/gtest.h>
#include "field.hpp"
#include "util/display.hpp"

using namespace fb;


void displayManualTestMessage()
{
    std::cout << "The following test must be checked manually:" << std::endl;
    std::cout << "Ask yourself whether things display as they should on screen." << std::endl;
    std::cout << "The success of this test might depend on the platform used." << std::endl;
    std::cout << "Google test is powerless at detecting whether this test went well or not" << std::endl;
    std::cout << std::endl;
}

void runTestAnsiCode(std::string effectName, std::string applyEffectCode,
                     std::string cancelEffectCode)
{
    while(effectName.length() < 14)
        { effectName += " "; }
    std::cout << effectName << "  ";
    std::cout << applyEffectCode << "Abcdefghi ";
    std::cout << cancelEffectCode << "   Abcdefghi";
    std::cout << useDefaultStyle() << std::endl;
}

void runTestAnsiColorCode(std::string colorNameA, std::string colorNameB)
{
    std::string effectName = colorNameA;
    while(effectName.length() < 10)
        { effectName += " "; }
    effectName += " x ";
    effectName += colorNameB;
    while(effectName.length() < 25)
        { effectName += " "; }
    std::cout << effectName << "  ";
    std::cout << setColor(colorNameA ,"Untouched") << "Abcdefghi " << unsetColor() << " ";
    std::cout << setColor(colorNameB ,"Untouched") << "Abcdefghi " << unsetColor() << " ";
    std::cout << setColor("Untouched",colorNameA ) << "Abcdefghi " << unsetColor() << " ";
    std::cout << setColor("Untouched",colorNameB ) << "Abcdefghi " << unsetColor() << " ";
    std::cout << setColor(colorNameA ,colorNameB ) << "Abcdefghi " << unsetColor() << " ";
    std::cout << setColor(colorNameB ,colorNameA ) << "Abcdefghi " << unsetColor() << " ";
    std::cout << "Abcdefghi";
    std::cout << useDefaultStyle() << std::endl;
}

TEST(util_display, ansiCodes)
{
    displayManualTestMessage();

    std::cout << "Effect name     With effect  Effect cancelled" << std::endl;
    std::cout << "─────────────────────────────────────────────" << std::endl;
    runTestAnsiCode("Bold"       , setBold()     , setNormalFontWeight());
    runTestAnsiCode("Faint"      , setFaint()    , setNormalFontWeight());
    runTestAnsiCode("Italic"     , setItalic()   , unsetItalic()        );
    std::cout << "---------------------------------------------" << std::endl;
    runTestAnsiCode("Underline"  , setUnderline(), unsetUnderline()     );
    std::cout << "---------------------------------------------" << std::endl;
    runTestAnsiCode("Overlined"  , setOverlined(), unsetOverlined()     );
    std::cout << "---------------------------------------------" << std::endl;
    runTestAnsiCode("Slow blink" , setSlowBlink(), unsetBlink()         );
    runTestAnsiCode("Crossed-out", setCrossedOut(), unsetCrossedOut()   );
    std::cout << std::endl << std::endl;

    std::cout << "Foreground and background colors" << std::endl;
    std::cout << std::endl;
    std::cout << "Color      x color alt.    Front=col. Front=alt. Back=col.  Back=alt.  Blended A  Blended B  Default" << std::endl;
    std::cout << "────────────────────────────────────────────────────────────────────────────────────────────────────" << std::endl;
    runTestAnsiColorCode("Black"     , "Dark Gray"   );
    runTestAnsiColorCode("Red"       , "Light Red"   );
    runTestAnsiColorCode("Green"     , "Light Green" );
    runTestAnsiColorCode("Brown"     , "Yellow"      );
    runTestAnsiColorCode("Blue"      , "Light Blue"  );
    runTestAnsiColorCode("Purple"    , "Light Purple");
    runTestAnsiColorCode("Cyan"      , "Light Cyan"  );
    runTestAnsiColorCode("Light Gray", "White"       );
    std::cout << "RGB        x RGB          "
              << " " << setRgbFrontColor(40,177,249)           << "Abcdefghi " << unsetColor()
              << " " << setRgbFrontColor(80,220,249)           << "Abcdefghi " << unsetColor()
              << " " << setRgbBackColor(40,177,249)            << "Abcdefghi " << unsetColor()
              << " " << setRgbBackColor(80,220,249)            << "Abcdefghi " << unsetColor()
              << " " << setRgbColor({40,177,249},{80,220,249}) << "Abcdefghi " << unsetColor()
              << " " << setRgbColor({80,220,249},{40,177,249}) << "Abcdefghi " << unsetColor()
              << " " << useDefaultStyle()                      << "Abcdefghi " << std::endl;

    std::cout << std::endl << std::endl;

    std::cout << "Non-recommended ANSI features because not supported widely enough" << std::endl;
    std::cout << std::endl;
    std::cout << "Effect name     With effect  Effect cancelled" << std::endl;
    std::cout << "─────────────────────────────────────────────" << std::endl;
    runTestAnsiCode("Fast blink", setFastBlink_unsupported(), unsetBlink());
    runTestAnsiCode("Framed"    , setFramed_unsupported()   , unsetFrames_unsupported());
    runTestAnsiCode("Circled"   , setCircled_unsupported()  , unsetFrames_unsupported());
    for(int iFont=0 ; iFont < 10 ; iFont++)
    {
        std::string effectName = "";
        effectName += "Set font " + std::to_string(iFont);
        runTestAnsiCode(effectName, setFont_unsupported(iFont), setDefaultFont_unsupported());
    }

    SUCCEED();
}

TEST(util_display, colorMaps)
{
    displayManualTestMessage();
    std::cout << "Displays colorbars:" << std::endl;
    ColorMap::listAvailableColorMaps();

    SUCCEED();
}

TEST(util_display, displayAsImage)
{
    displayManualTestMessage();

    Domain domain1D(Resolution(5), Point(0.0), Distance(1.0));
    Field field1D(domain1D, Anchor("C"), GhostCells(0,0));
    for(int i=0 ; i < 5 ; i++)
        { field1D(i) = double(i); }
    field1D.displayAsImage( ColorMap(Range(0.0,5.0),"viridis") );

    Domain domain2D(Resolution(5,4), Point(0.0,0.0), Distance(1.0,1.0));
    Field field2D(domain2D, Anchor("C"), GhostCells(0,0,0,0));

    Domain domain3D(Resolution(5,4,3), Point(0.0,0.0,0.0), Distance(1.0,1.0,1.0));
    Field field3D(domain3D, Anchor("C"), GhostCells(0,0,0,0,0,0));
    ASSERT_DEATH( field3D.displayAsImage( ColorMap(Range(0.0,1.0),"viridis") ),
                  "Field must not be three-dimensional, but it is");
}

