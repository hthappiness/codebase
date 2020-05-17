
class IPollEvent
{

};

/*
 *eg:
 *class CUdpSession:public IPollEvent
 *{
 *	
 *}
 *class CTcpSession:public IPollEvent
 *{
 *	
 *}
 *class CHttpSession:public IPollEvent
 *{
 *	
 *}
 *class CWebsocketSession:public IPollEvent
 *{
 *	
 *}
 *class CSerialSession:public IPollEvent
 *{
 *	
 *}
 */

class CPoller
{
public:
	CPoller();
	~CPoller();

	int addPollItem(IPollEvent*, )
};