/*
 * implement plugin strandard api
 * for boost dynamic import
 */
 
 #include "libdf.h"
 
 const std::string g_strPluginName    = "demo_plugin";
 const std::string g_strPluginVersion = "1.0.0";
 
 DF_API LONG PluginLoad()
 {
	 //register factory
	 dataFlow::RegisterProducerFactory(std::make_shared<>());
 }
 
 DF_API LONG PluginUnload()
 {
	 
 }
 
 DF_PAI std::string GetProducetName()
 {
	 return g_strPluginName;
 }
 
 DF_PAI std::string GetProducetVersion()
 {
	 return g_strPluginVersion;
 }