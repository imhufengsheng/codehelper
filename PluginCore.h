#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/timer.h>
#include <cbplugin.h>
#include <configmanager.h>
#include <manager.h>
#include <logmanager.h>
#include <sdk_events.h>

// CodeBlocks 插件核心类
class CodeBlocksAIPlugin : public cbPlugin
{
    DECLARE_EVENT_TABLE()

public:
    CodeBlocksAIPlugin();
    ~CodeBlocksAIPlugin();


    // 获取插件名称
    virtual const wxChar* GetName() const { return wxT("CodeBlocksAIPlugin"); }

    // 获取插件作者
    virtual const wxChar* GetAuthor() const { return wxT("Your Name"); }

    // 获取插件描述
    virtual const wxChar* GetDescription() const { return wxT("A plugin for CodeBlocks"); }

    virtual void BuildMenu(wxMenuBar* menuBar) {}
    virtual void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0) {}
    virtual bool BuildToolBar(wxToolBar* toolBar) { return false; }

protected:
    // 监听编辑器按键事件
    void OnEditorKeyPress(wxKeyEvent& event);  // 修改回原来的wxKeyEvent

    // 监听编辑器文本变化事件
    void OnEditorModified(CodeBlocksEvent& event);  // 修改函数名和参数类型

private:
    wxTimer m_timer;  // 计时器
    wxString m_currentLine;  // 当前行内容
    wxString m_virtualCompletion;  // 虚拟补全内容
};
