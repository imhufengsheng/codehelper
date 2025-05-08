#ifndef COMPLETIONDIALOG_H
#define COMPLETIONDIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <string>

class CompletionDialog : public wxDialog
{
public:
    // Constructor, which receives the parent window and generates code completions
    CompletionDialog(wxWindow* parent, const wxString& completion, const wxString& currentCode);

    // Destructors
    virtual ~CompletionDialog();

    // Get whether completions should be inserted.
    bool ShouldInsertCompletion() const { return m_shouldInsert; }

private:
    // Event handlers
    void OnShowButtonClick(wxCommandEvent& event);
    void OnInsertButtonClick(wxCommandEvent& event);
    void OnCloseButtonClick(wxCommandEvent& event);

private:
    wxTextCtrl* m_completionCtrl;      // A text control that displays completions
    wxButton* m_showButton;            // Show completion button
    wxButton* m_insertButton;          // Insert completion button
    wxButton* m_closeButton;           // Close completion button
    wxString m_completion;             // Save completion button
    bool m_shouldInsert;               // Mark whether to insert

    DECLARE_EVENT_TABLE()
};

#endif // COMPLETIONDIALOG_H
