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

        int currentLine = control->GetCurrentLine();

        control->AnnotationClearAll();

        control->StyleSetForeground(wxSCI_STYLE_LASTPREDEFINED + 1, wxColour(128, 128, 255));
        control->StyleSetBackground(wxSCI_STYLE_LASTPREDEFINED + 1, wxColour(240, 240, 240));
        control->StyleSetSize(wxSCI_STYLE_LASTPREDEFINED + 1, control->StyleGetSize(0));
        control->StyleSetFaceName(wxSCI_STYLE_LASTPREDEFINED + 1, control->StyleGetFaceName(0));

        control->AnnotationSetText(currentLine, completion);
        control->AnnotationSetStyle(currentLine, wxSCI_STYLE_LASTPREDEFINED + 1);
        control->AnnotationSetVisible(wxSCI_ANNOTATION_BOXED);
    }
}

void UserInterface::insertCompletion(const wxString& completion)
{
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    if (!edMan) return;

    cbEditor* editor = edMan->GetBuiltinActiveEditor();
    if (!editor) return;

    cbStyledTextCtrl* control = editor->GetControl();
    if (control) {
        control->AddText(completion);
    }
}
void UserInterface::clearCompletion()
{
    cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (editor)
    {
        cbStyledTextCtrl* control = editor->GetControl();
        if (control) {
            control->AnnotationClearAll();
        }
    }
}
