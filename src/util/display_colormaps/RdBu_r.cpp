
#include "util/display.hpp"

namespace fb
{

const std::vector<Rgb>
ColorMap::RdBu_r =
        {{5,48,97},
         {6,50,100},
         {7,52,103},
         {8,54,106},
         {9,56,109},
         {10,59,112},
         {12,61,115},
         {13,63,118},
         {14,65,121},
         {15,67,123},
         {16,69,126},
         {17,71,129},
         {18,73,132},
         {19,76,135},
         {20,78,138},
         {21,80,141},
         {23,82,144},
         {24,84,147},
         {25,86,150},
         {26,88,153},
         {27,90,156},
         {28,92,159},
         {29,95,162},
         {30,97,165},
         {31,99,168},
         {32,101,171},
         {34,103,172},
         {35,105,173},
         {36,106,174},
         {38,108,175},
         {39,110,176},
         {40,112,177},
         {42,113,178},
         {43,115,179},
         {44,117,180},
         {46,119,181},
         {47,121,181},
         {48,122,182},
         {50,124,183},
         {51,126,184},
         {52,128,185},
         {54,129,186},
         {55,131,187},
         {56,133,188},
         {58,135,189},
         {59,136,190},
         {60,138,190},
         {62,140,191},
         {63,142,192},
         {64,143,193},
         {66,145,194},
         {67,147,195},
         {70,149,196},
         {73,151,197},
         {76,153,198},
         {79,155,199},
         {82,157,200},
         {86,159,201},
         {89,161,202},
         {92,163,203},
         {95,165,205},
         {98,167,206},
         {101,169,207},
         {104,171,208},
         {107,172,209},
         {110,174,210},
         {113,176,211},
         {117,178,212},
         {120,180,213},
         {123,182,214},
         {126,184,215},
         {129,186,216},
         {132,188,217},
         {135,190,218},
         {138,192,219},
         {141,194,220},
         {144,196,221},
         {147,198,222},
         {150,199,223},
         {152,200,224},
         {155,201,224},
         {157,203,225},
         {160,204,226},
         {162,205,227},
         {165,206,227},
         {167,208,228},
         {169,209,229},
         {172,210,229},
         {174,211,230},
         {177,213,231},
         {179,214,232},
         {182,215,232},
         {184,216,233},
         {187,218,234},
         {189,219,234},
         {192,220,235},
         {194,221,236},
         {197,223,236},
         {199,224,237},
         {202,225,238},
         {204,226,239},
         {207,228,239},
         {209,229,240},
         {210,230,240},
         {212,230,241},
         {213,231,241},
         {215,232,241},
         {216,233,241},
         {218,233,242},
         {219,234,242},
         {221,235,242},
         {222,235,242},
         {224,236,243},
         {225,237,243},
         {227,237,243},
         {228,238,244},
         {230,239,244},
         {231,240,244},
         {233,240,244},
         {234,241,245},
         {236,242,245},
         {237,242,245},
         {239,243,245},
         {240,244,246},
         {242,245,246},
         {243,245,246},
         {245,246,247},
         {246,247,247},
         {247,246,246},
         {247,245,244},
         {248,244,242},
         {248,243,240},
         {248,242,239},
         {248,241,237},
         {249,240,235},
         {249,239,233},
         {249,238,231},
         {249,237,229},
         {249,235,227},
         {250,234,225},
         {250,233,223},
         {250,232,222},
         {250,231,220},
         {251,230,218},
         {251,229,216},
         {251,228,214},
         {251,227,212},
         {252,226,210},
         {252,224,208},
         {252,223,207},
         {252,222,205},
         {253,221,203},
         {253,220,201},
         {253,219,199},
         {253,217,196},
         {252,215,194},
         {252,213,191},
         {252,211,188},
         {251,208,185},
         {251,206,183},
         {251,204,180},
         {250,202,177},
         {250,200,175},
         {249,198,172},
         {249,196,169},
         {249,194,167},
         {248,191,164},
         {248,189,161},
         {248,187,158},
         {247,185,156},
         {247,183,153},
         {247,181,150},
         {246,179,148},
         {246,177,145},
         {246,175,142},
         {245,172,139},
         {245,170,137},
         {245,168,134},
         {244,166,131},
         {243,164,129},
         {242,161,127},
         {241,158,125},
         {240,156,123},
         {239,153,121},
         {238,150,119},
         {236,147,116},
         {235,145,114},
         {234,142,112},
         {233,139,110},
         {232,137,108},
         {230,134,106},
         {229,131,104},
         {228,128,102},
         {227,126,100},
         {226,123,98},
         {225,120,96},
         {223,118,94},
         {222,115,92},
         {221,112,89},
         {220,110,87},
         {219,107,85},
         {218,104,83},
         {216,101,81},
         {215,99,79},
         {214,96,77},
         {213,93,76},
         {211,90,74},
         {210,88,73},
         {208,85,72},
         {207,82,70},
         {206,79,69},
         {204,76,68},
         {203,73,66},
         {201,71,65},
         {200,68,64},
         {198,65,62},
         {197,62,61},
         {196,59,60},
         {194,56,58},
         {193,54,57},
         {191,51,56},
         {190,48,54},
         {189,45,53},
         {187,42,52},
         {186,40,50},
         {184,37,49},
         {183,34,48},
         {182,31,46},
         {180,28,45},
         {179,25,44},
         {177,24,43},
         {174,23,42},
         {171,22,42},
         {168,21,41},
         {165,20,41},
         {162,19,40},
         {159,18,40},
         {156,17,39},
         {153,16,39},
         {150,15,39},
         {147,14,38},
         {144,13,38},
         {141,12,37},
         {138,11,37},
         {135,10,36},
         {132,9,36},
         {129,8,35},
         {127,8,35},
         {124,7,34},
         {121,6,34},
         {118,5,33},
         {115,4,33},
         {112,3,32},
         {109,2,32},
         {106,1,31},
         {103,0,31}};

}
