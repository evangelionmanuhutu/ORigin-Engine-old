#include <Origin.h>
#include "..\Editor.h"

namespace Origin {

  class EditorBuild 
    : public Application
  {
  public:
    EditorBuild() 
      : Application("Editor", false, false)
    {
      Application::Get().GetWindow().SetIcon("assets/textures/icon_editor.png");
	    PushLayer(new Editor());
    }
    ~EditorBuild() {}
  };

  Application* CreateApplication()
  {
    OGN_CORE_INFO("Editor Created!");
    return new EditorBuild();
  };
}