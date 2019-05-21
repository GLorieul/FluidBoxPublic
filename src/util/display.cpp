
#include <map>
#include <string>
#include "util/display.hpp"

namespace fb
{

// Outputs a number formated into a 8 char-long std::string
std::string formatToFit(const double numberToDisplay)
{
    constexpr int nbCharsAvailable = 8;
    const double nb = numberToDisplay;
    const double maxDecimal = 99.999944;
    const double maxExp     = 9.944e99;
    char formattedNb[nbCharsAvailable];
    if((-maxDecimal < nb) and (nb < maxDecimal))
        { snprintf(&formattedNb[0], nbCharsAvailable+1, "%+8.4f", nb); }
    else if ((-maxExp < nb) and (nb < maxExp))
        { snprintf(&formattedNb[0], nbCharsAvailable+1, "%+8.1e", nb); }
    else
        { snprintf(&formattedNb[0], nbCharsAvailable+1, "%+8.0e", nb); }
    return std::string(&formattedNb[0]);
}



std::string setColor(std::string frontColorName, std::string backColorName)
{

    Dictionary<std::string> frontColors = 
            {{"Black"     , "30"}, {"Dark Gray"   , "90"},
             {"Red"       , "31"}, {"Light Red"   , "91"},
             {"Green"     , "32"}, {"Light Green" , "92"},
             {"Brown"     , "33"}, {"Yellow"      , "93"},
             {"Blue"      , "34"}, {"Light Blue"  , "94"},
             {"Purple"    , "35"}, {"Light Purple", "95"},
             {"Cyan"      , "36"}, {"Light Cyan"  , "96"},
             {"Light Gray", "37"}, {"White"       , "97"},
             {"Orange"    , "33"},  //Orange is synonym for brown
             {"Untouched" ,  "" }, {"Default", "39"}};

    Dictionary<std::string> backColors = 
            {{"Black"     , "40"}, {"Dark Gray"   , "100"},
             {"Red"       , "41"}, {"Light Red"   , "101"},
             {"Green"     , "42"}, {"Light Green" , "102"},
             {"Brown"     , "43"}, {"Yellow"      , "103"},
             {"Blue"      , "44"}, {"Light Blue"  , "104"},
             {"Purple"    , "45"}, {"Light Purple", "105"},
             {"Cyan"      , "46"}, {"Light Cyan"  , "106"},
             {"Light Gray", "47"}, {"White"       , "107"},
             {"Orange"    , "43"},  //Orange is synonym for brown
             {"Untouched" , ""}, {"Default", "49"}};
    const std::string frontColor = frontColors[frontColorName];
    const std::string backColor  = backColors[backColorName];
    const bool areTwoColors = ((frontColor!="") and (backColor!=""));
    std::string colorCode = frontColor;
    colorCode += areTwoColors ? ";" : "";
    colorCode += backColor;
    return generateAnsiCode(colorCode);
}

std::string setRgbColor(Rgb rgbFront, Rgb rgbBack)
{
    const bool areTwoColors = rgbFront.isInitialized and rgbBack.isInitialized;
    std::string colorCode = "";
    colorCode += rgbFront.getAnsiCode("front");
    colorCode += areTwoColors ? ";" : "";
    colorCode += rgbBack.getAnsiCode("back");
    return generateAnsiCode(colorCode);
}

const Dictionary<const ColorMap::ColorMapData*>
ColorMap::colorMaps =
        {{"magma"  , &ColorMap::magma  },
         {"inferno", &ColorMap::inferno},
         {"plasma" , &ColorMap::plasma },
         {"viridis", &ColorMap::viridis},
         {"binary" , &ColorMap::binary },
         {"jet"    , &ColorMap::jet    },
         {"RdBu_r" , &ColorMap::RdBu_r }};

void ColorMap::listAvailableColorMaps()
{
    for(auto cmData : ColorMap::colorMaps)
    {
        const std::string colorMapName = cmData.first;
        ColorMap cm(Range(0.0,1.0), colorMapName);
        std::cout << cm.toString() << std::endl;
    }
}


// IMPLEMENTATION OF ColorMap CLASS

std::string ColorMap::getColorBar(int colorBarLength) const
{
    std::string colorBar = "";
    const double step = (range_.max - range_.min) / double(colorBarLength);
    for(double val = range_.min ; val < (range_.max + 1e-4); val += step)
        { colorBar += writePixel(val); }
    colorBar += useDefaultStyle();
    return formatToFit(range_.min) + " " + colorBar + " "
         + formatToFit(range_.max);
}

std::string ColorMap::writePixel(double value) const
{
    int iTone = int(coefDir_*(value - range_.min));
    iTone = max(iTone,0);
    iTone = min(iTone,255);
    return setRgbBackColor((*cmData_)[iTone]) + " ";
}


}

