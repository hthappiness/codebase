
#include <wchar.h>
#include <fttypes.h>
#include <freetype.h>
#include <ftglyph.h>

namespace vias{

class CFreeType{
public:
    CFreeType();
    ~CFreeType();

    int init();
    int newBitmap();
    
private:
    FT_Library m_pFTLib   = nullptr;
    FT_Face m_pFTFace     = nullptr;

    std::string m_fontPath;
};

}