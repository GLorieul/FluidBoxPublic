#ifndef UTIL__DISPLAY___HPP
#define UTIL__DISPLAY___HPP

#include <cassert>
#include <map>
#include <string>
#include <vector>
#include "util/error.hpp"
#include "util/struct.hpp"

namespace fb
{
// Outputs a number formated into a 8 char-long std::string
std::string formatToFit(const double numberToDisplay);


struct Rgb
{
    Rgb()
        : r(0), g(0), b(0), isInitialized(false)
    {/*Do nothig*/}

    Rgb(unsigned char r, unsigned char g, unsigned char b)
        :r(r), g(g), b(b), isInitialized(true)
    {/*Do nothing*/}

    Rgb(std::vector<unsigned char> values)
        : Rgb(values[0], values[1], values[2])
    {/*Do nothing*/}

    std::string getAnsiCode(std::string type) const
    {
        if(this->isInitialized)
        {
            std::string redStr   = std::to_string(r);
            std::string greenStr = std::to_string(g);
            std::string blueStr  = std::to_string(b);
            std::string rgbStr   = redStr + ";" + greenStr + ";" + blueStr;
            if     (type=="front") { return "38;2;"+rgbStr; }
            else if(type=="back")  { return "48;2;"+rgbStr; }
            else
            {
                std::string msg = "Illegal value for \"type\". ";
                msg            += "Legal values are \"front\" and \"back\"";
                raiseError(__FILE__, __LINE__, __FUNCTION__, msg);
            }
        }
        else
            { return ""; }
    }

    unsigned char r, g, b;
    bool isInitialized;
};

static std::string generateAnsiCode(std::string code)
    { return std::string(1,'\x1B') + "[" + code + "m"; }

// For a list of codes, refer to
// https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
static std::string useDefaultStyle()     { return generateAnsiCode( "0"); }
static std::string setBold()             { return generateAnsiCode( "1"); }
static std::string setFaint()            { return generateAnsiCode( "2"); }
static std::string setNormalFontWeight() { return generateAnsiCode("22"); }
static std::string setItalic()           { return generateAnsiCode( "3"); }
static std::string unsetItalic()         { return generateAnsiCode("23"); }
static std::string setUnderline()        { return generateAnsiCode( "4"); }
static std::string unsetUnderline()      { return generateAnsiCode("24"); }
static std::string setOverlined()        { return generateAnsiCode("53"); }
static std::string unsetOverlined()      { return generateAnsiCode("55"); }
static std::string setSlowBlink()        { return generateAnsiCode( "5"); }
static std::string unsetBlink()          { return generateAnsiCode("25"); }
static std::string setCrossedOut()       { return generateAnsiCode( "9"); }
static std::string unsetCrossedOut()     { return generateAnsiCode("29"); }
static std::string unsetColor()          { return generateAnsiCode("39;49"); }
// I'm not implementing the "reverse video" effect because it is ugly anyway

std::string setColor(std::string frontColorName,
                     std::string backColorName="Untouched");
std::string setRgbColor(Rgb rgbFront, Rgb rgbBack);

static std::string setRgbFrontColor(Rgb rgb)
    { return setRgbColor(rgb, Rgb()); }
static std::string setRgbBackColor(Rgb rgb)
    { return setRgbColor(Rgb(), rgb); }
static std::string setRgbFrontColor(const int r, const int g, const int b)
    { return setRgbColor(Rgb(r,g,b), Rgb()); }
static std::string setRgbBackColor(const int r, const int g, const int b)
    { return setRgbColor(Rgb(), Rgb(r,g,b)); }


// NON-SUPPORTED ANSI CODES
// because they are not supported widely enough.
// I kept the implementation for curiosity + for fun
// (and because it should not require much maintenance)
static std::string setFastBlink_unsupported()        { return generateAnsiCode( "6"); }
static std::string setFont_unsupported(int iFont)
{
    assert((0 <= iFont) and (iFont <= 9));
    std::string code = std::to_string(10 + iFont);
    return generateAnsiCode("11"); 
}
static std::string setDefaultFont_unsupported()      { return generateAnsiCode("10"); }
static std::string setFramed_unsupported()           { return generateAnsiCode("51"); }
static std::string setCircled_unsupported()          { return generateAnsiCode("52"); }
static std::string unsetFrames_unsupported()         { return generateAnsiCode("54"); } //Also for Circled


// Class that allows to write a tile of the console in a given color
class ColorMap
{
public:
    static constexpr int nbTones = 255;
    ColorMap(Range rangeColorMap, std::string colorMap="viridis")
        :range_(rangeColorMap), colorMapName_(colorMap),
         cmData_(colorMaps[colorMap]),
         coefDir_(nbTones/(range_.max - range_.min))
    {/*Do nothing*/}

    std::string getColorBar(int colorBarLength=32) const;

    std::string toString() const
        { return getColorBar() + useDefaultStyle() + " " + colorMapName_; }

    std::string writePixel(double value) const;

public:
    typedef std::vector<Rgb>  ColorMapData;
    static const ColorMapData magma;
    static const ColorMapData inferno;
    static const ColorMapData plasma;
    static const ColorMapData viridis;
    static const ColorMapData binary;
    static const ColorMapData jet;
    static const ColorMapData RdBu_r;

    static const Dictionary<const ColorMap::ColorMapData*> colorMaps;

    static void listAvailableColorMaps();


private:
    std::string colorMapName_;
    Range range_;
    const ColorMapData* cmData_;
    double coefDir_; //Helps in conversion from value to tone index
};



}

#endif

