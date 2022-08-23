#include <Origin.h>
#include "..\Sandbox2D.h"

namespace Origin {

  class SandboxBuild : public Application
  {
  public:
    SandboxBuild() : Application("Sandbox", true, false)
    {
	    PushLayer(new Sandbox2D());
    }
    ~SandboxBuild() {}
  };

  Application* CreateApplication()
  {
    OGN_CORE_INFO("Sandbox Created!");
    return new SandboxBuild();
  };
}