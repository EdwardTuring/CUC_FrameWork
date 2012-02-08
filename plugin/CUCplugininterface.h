
#include <QtCore>
#include <QtPlugin>
namespace Plugin
{
    class CUCPluginInterface;
}
class CUCPluginInterface
 {
 public:
    virtual ~CUCPluginInterface() {}

    virtual void debug() const = 0;
    virtual QString pluginName() const =0;
    virtual QString pluginVersion() const =0;
    virtual QString pluginIcoUrl() const =0;



 };
Q_DECLARE_INTERFACE(CUCPluginInterface,"cn.edu.cuc.CUCPluginInterface/0.1")
