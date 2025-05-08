#include <wx/wx.h>
#include <cbplugin.h>
#include <sdk_events.h>
#include <memory>
#include <sdk.h>
#include <configurationpanel.h>
#include <wx/menu.h>

class CodeBlocksAIPlugin : public cbPlugin
{
     wxDECLARE_EVENT_TABLE()

public:

    CodeBlocksAIPlugin();


    virtual ~CodeBlocksAIPlugin();


    virtual const wxChar* GetName() const  { return wxT("codehelper"); }


    virtual const wxChar* GetAuthor() const  { return wxT("neu"); }


    virtual const wxChar* GetDescription() const  {
        return wxT("AI-powered code completion plugin for Code::Blocks");
    }

public:

    bool GetCompletionState() const { return m_completionActive; }


    virtual void OnAttach();


    virtual void OnRelease() {}

private:

    void ProcessShortcutKeyShow(wxCommandEvent& event);


    void ProcessShortcutKeyInsert(wxCommandEvent& event);

    void ShowAIDialog(wxCommandEvent& event);

private:
    wxString m_currentLine;
    wxString m_virtualCompletion;
    bool m_completionActive;
    wxString m_pendingCompletion;
};
