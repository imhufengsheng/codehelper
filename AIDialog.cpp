#include "AIDialog.h"
#include "NetworkRequest.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include "../include/nlohmann/json.hpp"

using json = nlohmann::json;

enum {
    ID_SEND_BUTTON = wxID_HIGHEST + 1,
    ID_CLOSE_BUTTON
};

BEGIN_EVENT_TABLE(AIDialog, wxDialog)
    EVT_BUTTON(ID_SEND_BUTTON, AIDialog::OnSendButtonClick)
    EVT_BUTTON(ID_CLOSE_BUTTON, AIDialog::OnCloseButtonClick)
END_EVENT_TABLE()

AIDialog::AIDialog(wxWindow* parent, const wxString& code)
    : wxDialog(parent, wxID_ANY, wxT("AI Code Assistant"), wxDefaultPosition, wxSize(600, 500),
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      m_code(code)
{
    // Create dialog layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add description text
    wxStaticText* infoText = new wxStaticText(this, wxID_ANY,
        wxT("Please enter your question. The AI assistant will analyze the current code context:"));
    mainSizer->Add(infoText, 0, wxEXPAND | wxALL, 10);

    // Create question input area
    wxStaticText* questionLabel = new wxStaticText(this, wxID_ANY, wxT("Question:"));
    mainSizer->Add(questionLabel, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    m_questionCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    mainSizer->Add(m_questionCtrl, 1, wxEXPAND | wxALL, 10);

    // Create response display area
    wxStaticText* responseLabel = new wxStaticText(this, wxID_ANY, wxT("Response:"));
    mainSizer->Add(responseLabel, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    m_responseCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    mainSizer->Add(m_responseCtrl, 2, wxEXPAND | wxALL, 10);

    // Create button area
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    m_sendButton = new wxButton(this, ID_SEND_BUTTON, wxT("Send"));
    m_closeButton = new wxButton(this, ID_CLOSE_BUTTON, wxT("Close"));

    buttonSizer->Add(m_sendButton, 0, wxALL, 5);
    buttonSizer->Add(m_closeButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);

    SetSizer(mainSizer);
    Layout();
    Centre();
}

AIDialog::~AIDialog()
{
    // Destructor, no special handling needed
}

wxString AIDialog::GetResponseText() const
{
    return m_responseCtrl->GetValue();
}

void AIDialog::OnSendButtonClick(wxCommandEvent& event)
{
    wxString question = m_questionCtrl->GetValue();
    if (question.IsEmpty())
    {
        wxMessageBox(wxT("Please enter a question"), wxT("Notice"), wxICON_INFORMATION);
        return;
    }

    // Disable send button to prevent multiple clicks
    m_sendButton->Disable();
    m_responseCtrl->SetValue(wxT("Processing..."));

    // Send request to Ollama and get response
    wxString response = SendToOllama(question);
    m_responseCtrl->SetValue(response);

    // Re-enable send button
    m_sendButton->Enable();
}

void AIDialog::OnCloseButtonClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

wxString AIDialog::SendToOllama(const wxString& question)
{
    std::string url = "http://localhost:11434/api/generate";

    // Build prompt, including code context and user question
    std::string prompt = "The following is the code in the current editor:\n\n";
    prompt += std::string(m_code.mb_str());
    prompt += "\n\nUser question:";
    prompt += std::string(question.mb_str());

    // Build request JSON
    json request = {
        {"model", "qwen2.5-coder:0.5b"},
        {"prompt", prompt},
        {"stream", false}
    };

    std::string response;
    if (NetworkRequest::post(url, request.dump(), response))
    {
        try {
            auto responseJson = json::parse(response);
            if (responseJson.contains("response")) {
                return wxString(responseJson["response"].get<std::string>());
            } else {
                return wxT("Error: API response does not contain 'response' field");
            }
        }
        catch (const json::parse_error& e) {
            return wxString::Format(wxT("JSON parsing error: %s"), e.what());
        }
    } else {
        return wxT("Failed to connect to Ollama service. Please ensure Ollama is running and the qwen2.5-coder:0.5b model is loaded");
    }
}
