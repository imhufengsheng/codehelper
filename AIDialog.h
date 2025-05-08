#ifndef AIDIALOG_H
#define AIDIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <string>


class AIDialog : public wxDialog
{
public:

    AIDialog(wxWindow* parent, const wxString& code);


    virtual ~AIDialog();


    wxString GetResponseText() const;

private:

    void OnSendButtonClick(wxCommandEvent& event);


    void OnCloseButtonClick(wxCommandEvent& event);


    wxString SendToOllama(const wxString& question);

private:
    wxTextCtrl* m_questionCtrl;
    wxTextCtrl* m_responseCtrl;
    wxButton* m_sendButton;
    wxButton* m_closeButton;
    wxString m_code;

    DECLARE_EVENT_TABLE()
};

#endif // AIDIALOG_H
