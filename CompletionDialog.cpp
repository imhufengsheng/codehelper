#include "CompletionDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <manager.h>
#include <editormanager.h>
#include <cbeditor.h>
#include <cbstyledtextctrl.h>

enum {
    ID_SHOW_BUTTON = wxID_HIGHEST + 1,
    ID_INSERT_BUTTON,
    ID_CLOSE_BUTTON
};

BEGIN_EVENT_TABLE(CompletionDialog, wxDialog)
    EVT_BUTTON(ID_SHOW_BUTTON, CompletionDialog::OnShowButtonClick)
    EVT_BUTTON(ID_INSERT_BUTTON, CompletionDialog::OnInsertButtonClick)
    EVT_BUTTON(ID_CLOSE_BUTTON, CompletionDialog::OnCloseButtonClick)
END_EVENT_TABLE()

CompletionDialog::CompletionDialog(wxWindow* parent, const wxString& completion, const wxString& currentCode)
    : wxDialog(parent, wxID_ANY, wxT("Code Completion"), wxDefaultPosition, wxSize(800, 600),
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      m_completion(completion),
      m_shouldInsert(false)
{
    // Create dialog layout with a horizontal split
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Left panel for current code
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* currentCodeLabel = new wxStaticText(this, wxID_ANY, wxT("Current Code:"));
    leftSizer->Add(currentCodeLabel, 0, wxEXPAND | wxALL, 5);

    wxTextCtrl* currentCodeCtrl = new wxTextCtrl(this, wxID_ANY, currentCode,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    leftSizer->Add(currentCodeCtrl, 1, wxEXPAND | wxALL, 5);

    // Right panel for completion
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* completionLabel = new wxStaticText(this, wxID_ANY, wxT("AI Generated Code:"));
    rightSizer->Add(completionLabel, 0, wxEXPAND | wxALL, 5);

    m_completionCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    rightSizer->Add(m_completionCtrl, 1, wxEXPAND | wxALL, 5);

    // Add both panels to main sizer
    mainSizer->Add(leftSizer, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(rightSizer, 1, wxEXPAND | wxALL, 5);

    // Create button area at the bottom
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_showButton = new wxButton(this, ID_SHOW_BUTTON, wxT("Show Code"));
    m_insertButton = new wxButton(this, ID_INSERT_BUTTON, wxT("Insert Code"));
    m_closeButton = new wxButton(this, ID_CLOSE_BUTTON, wxT("Close"));

    buttonSizer->Add(m_showButton, 0, wxALL, 5);
    buttonSizer->Add(m_insertButton, 0, wxALL, 5);
    buttonSizer->Add(m_closeButton, 0, wxALL, 5);

    // Add button sizer to right panel
    rightSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);

    SetSizer(mainSizer);
    Layout();
    Centre();
}

CompletionDialog::~CompletionDialog()
{
}

void CompletionDialog::OnShowButtonClick(wxCommandEvent& event)
{
    // Show code completion
    m_completionCtrl->SetValue(m_completion);
}

void CompletionDialog::OnInsertButtonClick(wxCommandEvent& event)
{
    // Get the active editor
    EditorManager* edMan = Manager::Get()->GetEditorManager();
    cbEditor* ed = edMan->GetBuiltinActiveEditor();
    if (ed)
    {
        // Get the control and clear all content
        cbStyledTextCtrl* control = ed->GetControl();
        if (control)
        {
            control->ClearAll();
        }
    }
    
    // Mark should be inserted into completions.
    m_shouldInsert = true;
    EndModal(wxID_OK);
}

void CompletionDialog::OnCloseButtonClick(wxCommandEvent& event)
{
    // Close Dialoguebox
    EndModal(wxID_CANCEL);
}
