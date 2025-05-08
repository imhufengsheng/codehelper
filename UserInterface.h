#include <string>
#include <wx/string.h>


class UserInterface
{
public:

    static void showVirtualCompletion(const wxString& completion);


    static void insertCompletion(const wxString& completion);

    static void clearCompletion();
};
