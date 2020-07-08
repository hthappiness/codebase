
#include "base_type.h"

/* 支持随机访问 */
/* 支持顺序访问 */
class CBitStream
{
public:
	char operator[](U32 u32Offset)
	{
		return m_buf[u32Offset];
	}
private:
	char* m_buf;
	U32 m_u32bufLen;
	U32 m_u32offset;
};

class CNaluStream :public CBitStream
{
	
};