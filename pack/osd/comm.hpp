namespace vias
{
    INT UTF8_To_Unicode(CHAR *pcStr, INT iStrLen)
    {
        U8 u8Offset = 0; 
        U16 unicode;
        INT iUnicodeLen = 0;

        while ( pcStr[u8Offset]!= 0 )
        {
            unicode = pcStr[u8Offset]&0xff; 
            LOGI("first = 0x%x offset = %u strlen = %u", unicode, u8Offset, iStrLen);
            if (unicode >= 0xF0) 
            {          
                unicode = (unsigned short) (pcStr[u8Offset] & 0x07) << 18;          
                unicode |= (unsigned short) (pcStr[u8Offset + 1] & 0x3F) << 12;          
                unicode |= (unsigned short) (pcStr[u8Offset + 2] & 0x3F) << 6;          
                unicode |= (unsigned short) (pcStr[u8Offset + 3] & 0x3F);
                u8Offset += 4;
            } 
            else if (unicode >= 0xE0) 
            {          
                unicode = (unsigned short) (pcStr[u8Offset] & 0x0F) << 12;          
                unicode |= (unsigned short) (pcStr[u8Offset + 1] & 0x3F) << 6;          
                unicode |= (unsigned short) (pcStr[u8Offset + 2] & 0x3F); 
                u8Offset += 3;
            } 
            else if (unicode >= 0xC0)
            {          
                unicode = (unsigned short) (pcStr[u8Offset] & 0x1F) << 6;          
                unicode |= (unsigned short) (pcStr[u8Offset + 1] & 0x3F);
                u8Offset += 2;
            }
            else
            {
                unicode = pcStr[u8Offset];
                u8Offset++;
            }
        }

        return iUnicodeLen;
    }

}