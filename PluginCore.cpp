#include "PluginCore.h"
#include "CodeGenerator.h"
#include "UserInterface.h"
#include "AIDialog.h"
#include "CompletionDialog.h"
#include <manager.h>
#include <editormanager.h>
#include <cbeditor.h>
#include <cbstyledtextctrl.h>
#include <editorbase.h>
#include<wx/wx.h>
// Menu item IDs for identifying different shortcut keys
const int ID_EXPORT_SHOW = wxNewId();
const int ID_AI_DIALOG = wxNewId();

wxBEGIN_EVENT_TABLE(CodeBlocksAIPlugin, cbPlugin)
    EVT_MENU(ID_EXPORT_SHOW, CodeBlocksAIPlugin::ProcessShortcutKeyShow)
    EVT_MENU(ID_AI_DIALOG, CodeBlocksAIPlugin::ShowAIDialog)
wxEND_EVENT_TABLE()

CodeBlocksAIPlugin::CodeBlocksAIPlugin()
    : m_completionActive(false){}

CodeBlocksAIPlugin::~CodeBlocksAIPlugin()
{}


void CodeBlocksAIPlugin::OnAttach()
{
    // Add menu items
    wxMenu* menu = new wxMenu();
    menu->Append(ID_AI_DIALOG, wxT("Open AI Assistance"), wxT("Chat with AI"));
    menu->Append(ID_EXPORT_SHOW, wxT("Code Completion"), wxT("Show AI code completion"));

    // Add menu to plugins menu
    wxMenuBar* menuBar = Manager::Get()->GetAppFrame()->GetMenuBar();
    int pluginsPos = menuBar->FindMenu(wxT("Plugins"));
    if (pluginsPos != wxNOT_FOUND)
    {
        wxMenu* pluginsMenu = menuBar->GetMenu(pluginsPos);
        pluginsMenu->Append(wxID_ANY, wxT("AI Code Assistance"), menu);
    }

    // Add toolbar buttons
    wxBitmap aiIcon(16, 16);
    wxMemoryDC dc;
    dc.SelectObject(aiIcon);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.DrawCircle(8, 8, 6);
    dc.SelectObject(wxNullBitmap);
    
    wxBitmap showIcon(16, 16);
    wxMemoryDC dc2;
    dc2.SelectObject(showIcon);
    dc2.SetBackground(*wxWHITE_BRUSH);
    dc2.Clear();
    dc2.SetBrush(*wxGREEN_BRUSH);
    dc2.DrawCircle(8, 8, 6);
    dc2.SelectObject(wxNullBitmap);
    
    wxBitmap insertIcon(16, 16);
    wxMemoryDC dc3;
    dc3.SelectObject(insertIcon);
    dc3.SetBackground(*wxWHITE_BRUSH);
    dc3.Clear();
    dc3.SetBrush(*wxRED_BRUSH);
    dc3.DrawCircle(8, 8, 6);
    dc3.SelectObject(wxNullBitmap);

    wxFrame* frame = dynamic_cast<wxFrame*>(Manager::Get()->GetAppWindow());
    if (frame)
    {
        wxToolBar* toolbar = frame->GetToolBar();
        if (toolbar)
        {
            toolbar->AddTool(ID_AI_DIALOG, wxT("AI Assistant"), aiIcon, wxT("Open AI Assistant"));
            toolbar->AddTool(ID_EXPORT_SHOW, wxT("Code Completion"), showIcon, wxT("Show code completion"));
            toolbar->Realize();
        }
    }
    
    // Log message to show successful plugin attachment
    wxLogMessage("CodeHelper has been attached. Use toolbar buttons for AI assistance, showing and inserting code completions.");}


void CodeBlocksAIPlugin::ProcessShortcutKeyShow(wxCommandEvent& event) // Handle Shift+Space shortcut
{
    try {
        // Get editor and control
        cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        if (!editor) return;

        cbStyledTextCtrl* control = editor->GetControl();
        if (!control) return;

        // Get current cursor position
        int currentPos = control->GetCurrentPos();
        // Get text before cursor
        wxString textBeforeCursor = control->GetTextRange(0, currentPos);

        std::string generatedCode = CodeGenerator::generateCode(std::string(textBeforeCursor.mb_str())); // Generate code
        if (!generatedCode.empty()) {
            m_virtualCompletion = wxString(generatedCode); // Store virtual completion
            
            // Create and show completion dialog
            CompletionDialog dlg(Manager::Get()->GetAppWindow(), m_virtualCompletion, textBeforeCursor);
            if (dlg.ShowModal() == wxID_OK && dlg.ShouldInsertCompletion()) {
                // If user clicked Insert button, insert the completion
                UserInterface::insertCompletion(m_virtualCompletion);
                m_virtualCompletion.Clear();
                m_completionActive = false;
            } else {
                // Store completion for later use
                m_completionActive = true;
            }
        }
    } catch (const std::exception& e) {
        wxLogError("Exception occurred: %s", e.what());
    }
}


void CodeBlocksAIPlugin::ShowAIDialog(wxCommandEvent& event) // Handle Ctrl+Space shortcut
{
    try {
        // Get editor and control
        cbEditor* editor = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        if (!editor) return;

        cbStyledTextCtrl* control = editor->GetControl();
        if (!control) return;

        // Get current editor text
        wxString editorText = control->GetText();

        // Create and show AI dialog
        AIDialog dlg(Manager::Get()->GetAppWindow(), editorText);
        dlg.ShowModal();
    } catch (const std::exception& e) {
        wxLogError("Exception occurred in AI Dialog: %s", e.what());
    }
}


