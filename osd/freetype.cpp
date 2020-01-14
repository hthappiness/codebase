#include "freetype.hpp"

namespace vias{

    CFreeType::CFreeType()
    {

    }

    CFreeType::~CFreeType()
    {
        if( nullptr == m_pFTFace )
        {
            /* free face */
            FT_Done_Face(m_pFTFace);
        }

        if( nullptr == m_pFTLib )
        {
            /* free FreeType Lib */
            FT_Done_FreeType(m_pFTLib);
        }
    }

    int CFreeType::init()
    {
        FT_Error error;

        error = FT_Init_FreeType(&pFTLib);
        if(error)
        {
            LOGI("FT_Init_FreeType failed!\n");
            return -1;
        }

        error = FT_New_Face(pFTLib, pcFont, 0, &pFTFace);
        if(error)
        {
            LOGI("FT_New_Face failed!\n");
            goto END1;
        }
        FT_Select_Charmap(pFTFace, ft_encoding_unicode);
    }

    int CFreeType::newBitMap()
    {
         FT_UInt glyph_index;
        FT_Error error = 0;
        FT_Glyph glyph;
        FT_BitmapGlyph bitmap_glyph;
        FT_Bitmap *pbitmap;
	    FT_ULong ftUlong;
        INT i, j; 
        INT iRow = 0, iCol = 0;
        U32 u32stride = pstStr2Bitmap->u32stride;
        U32 u32ColorLen = 2;
        U32 u32FrtColor = pstStr2Bitmap->u32FrtColor;

        if (OSD_COLOR_FMT_RGB8888 == pstStr2Bitmap->enOsdClrFmt)
        {
            u32ColorLen = 4;
        }

        /* 设置大小 */
        error = FT_Set_Char_Size(m_pFTFace, pstStr2Bitmap->width * 64, 
                                pstStr2Bitmap->height * 64, 
                                pstStr2Bitmap->u32HorzReso, 
                                pstStr2Bitmap->u32VertReso);
        if(0 != error)
        {
            ALOGI("FT_Set_Char_Size failed!\n");
            return -1;
        }

        /* 根据unicode寻找graph */
        glyph_index = FT_Get_Char_Index(m_pFTFace, (FT_ULong)u16Unicode);
        if (0 == glyph_index)
        {
            LOGI("FT_Get_Char_Index failed!\n");
            return -1;
        }

        /* 加载graph */
        FT_Load_Glyph(m_pFTFace, glyph_index, FT_LOAD_MONOCHROME | FT_LOAD_RENDER);

        /* 获取graph */
        error = FT_Get_Glyph(m_pFTFace->glyph, &glyph);
        if(error)
        {
            LOGI("FT_Get_Glyph failed!\n");
            return -1;
        }
    
        /* 根据graph生成位图 */
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 0);

        bitmap_glyph = (FT_BitmapGlyph)glyph;
        pbitmap = &bitmap_glyph->bitmap;

        LOGI("top %u, left %u \r\n", bitmap_glyph->top, bitmap_glyph->left);

        /* 根据海思提供的接口，修改pstStr2Bitmap->u32FrtColor */
        if( (0 != pbitmap->rows)&& (0 != pbitmap->width))
        {
            Hisi_IsRegion_ReverseColor(pstStr2Bitmap->pData, pstStr2Bitmap->u32StartX, pstStr2Bitmap->u32StartY + (pstStr2Bitmap->u32CanvasHeight/2 - bitmap_glyph->top),
                pbitmap->width, pbitmap->rows, pstStr2Bitmap->enPixedFmt, &u32FrtColor);
        }

        /* 前景、背景图 */
        LOGI("FrtColor =0x%x background color = 0x%x", u32FrtColor, pstStr2Bitmap->u32BakColor);
        for (iRow = 0 ,j = pstStr2Bitmap->u32StartY + pstStr2Bitmap->u32CanvasHeight/2 - bitmap_glyph->top;
            iRow < pbitmap->rows; iRow++, j++)
        {
            for(iCol = 0, i = pstStr2Bitmap->u32StartX; iCol < pbitmap->width; iCol++, i++)
            {
                if((pbitmap->buffer[iRow * pbitmap->pitch + (iCol >>3) ] & (0xC0 >> (iCol & 0x7))) == 0)
                {
                    memcpy(pstStr2Bitmap->pRGBBuffer + i * u32ColorLen + j * u32stride, 
                        &pstStr2Bitmap->u32BakColor, u32ColorLen);
                }
                else
                {
                    memcpy(pstStr2Bitmap->pRGBBuffer + i * u32ColorLen + j * u32stride, 
                        &u32FrtColor, u32ColorLen); 
                }
            }
        }

        pstStr2Bitmap->u32FrtColor = u32FrtColor;
        
        if (pbitmap->width == 0)
        {
            pstStr2Bitmap->u32StartX += pstStr2Bitmap->u32Distance * 10;
        }
        else
        {
            pstStr2Bitmap->u32StartX += pbitmap->width + pstStr2Bitmap->u32Distance;
        }
    
        /* free glyph */
        FT_Done_Glyph(glyph);  
        glyph  =  NULL;  

    }

    
}