#include <Origin.h>
#include "..\Sandbox3D.h"

namespace Origin {

  class SandboxBuild : public Application
  {
  public:
    SandboxBuild() : Application("Sandbox 3D", false, false)
    {
      Application::Get().GetWindow().SetIcon("assets/textures/icon_origin.png");
	    PushLayer(new Sandbox3D());
    }
    ~SandboxBuild() {}
  };

  Application* CreateApplication()
  {
    OGN_CORE_INFO("Sandbox Created!");
    return new SandboxBuild();
  };
}