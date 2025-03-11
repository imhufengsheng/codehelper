#include "PluginCore.h"
#include "CodeGenerator.h"
#include "UserInterface.h"
#include <manager.h>
#include <editormanager.h>
#include <cbeditor.h>
#include <cbstyledtextctrl.h>

BEGIN_EVENT_TABLE(CodeBlocksAIPlugin, cbPlugin)
    EVT_KEY_DOWN(CodeBlocksAIPlugin::OnEditorKeyPress)
END_EVENT_TABLE()

CodeBlocksAIPlugin::CodeBlocksAIPlugin()
{
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_MODIFIED,
        new cbEventFunctor<CodeBlocksAIPlugin, CodeBlocksEvent>(this, &CodeBlocksAIPlugin::OnEditorModified));

    // 获取编辑器并绑定键盘事件
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    if (edMan) {
        cbEditor* editor = edMan->GetBuiltinActiveEditor();
        if (editor) {
            editor->GetControl()->Connect(wxEVT_KEY_DOWN,
                wxKeyEventHandler(CodeBlocksAIPlugin::OnEditorKeyPress),
                NULL, this);
        }
    }
}

CodeBlocksAIPlugin::~CodeBlocksAIPlugin()
{
}

void CodeBlocksAIPlugin::OnEditorModified(CodeBlocksEvent& event)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    if (!edMan) return;

    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor) return;

    cbStyledTextCtrl* control = editor->GetControl();
    if (!control) return;

    // 获取当前行文本并生成补全
    wxString currentLine = control->GetLine(control->GetCurrentLine());
    m_currentLine = currentLine;
    m_virtualCompletion = CodeGenerator::generateCode(m_currentLine.ToStdString());

    if (!m_virtualCompletion.IsEmpty()) {
        UserInterface::showVirtualCompletion(m_virtualCompletion);
    }

    event.Skip();
}

void CodeBlocksAIPlugin::OnEditorKeyPress(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_TAB)
    {
        // 如果用户按下 Tab 键，插入虚拟补全内容
        if (!m_virtualCompletion.IsEmpty())
        {
            UserInterface::insertCompletion(m_virtualCompletion);
            m_virtualCompletion.Clear();  // 清空虚拟补全内容
        }
    }
    event.Skip();  // 继续传播事件
}
