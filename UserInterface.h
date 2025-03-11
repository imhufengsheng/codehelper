#include <string>
#include <wx/string.h>

// 用户界面类
class UserInterface
{
public:
    // 显示虚拟补全内容
    static void showVirtualCompletion(const wxString& completion);
    // 插入补全内容
    static void insertCompletion(const wxString& completion);
};
