#include "UserInterface.h"
#include <wx/app.h>
#include <wx/config.h>
#include <wx/string.h>
#include <manager.h>
#include <configmanager.h>
#include <editormanager.h>
#include <cbeditor.h>
#include <cbstyledtextctrl.h>

void UserInterface::showVirtualCompletion(const wxString& completion)
{
    cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (editor)
    {
        cbStyledTextCtrl* control = editor->GetControl();
        if (!control) return;

        // 获取当前行
        int currentLine = control->GetCurrentLine();

        // 清除所有已有的注释
        control->AnnotationClearAll();

        // 设置注释样式
        control->StyleSetForeground(wxSCI_STYLE_LASTPREDEFINED + 1, wxColour(128, 128, 255));  // 蓝色文本
        control->StyleSetBackground(wxSCI_STYLE_LASTPREDEFINED + 1, wxColour(240, 240, 240));  // 浅灰背景
        control->StyleSetSize(wxSCI_STYLE_LASTPREDEFINED + 1, control->StyleGetSize(0));  // 与正常文本相同大小
        control->StyleSetFaceName(wxSCI_STYLE_LASTPREDEFINED + 1, control->StyleGetFaceName(0));  // 相同字体

        // 设置注释显示
        control->AnnotationSetText(currentLine, completion);
        control->AnnotationSetStyle(currentLine, wxSCI_STYLE_LASTPREDEFINED + 1);
        control->AnnotationSetVisible(wxSCI_ANNOTATION_BOXED);  // 使用方框样式显示
    }
}

void UserInterface::insertCompletion(const wxString& completion)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    if (!edMan) return;

    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor) return;

    cbStyledTextCtrl* control = editor->GetControl();
    if (control)
    {
        control->AddText(completion);
    }
}
