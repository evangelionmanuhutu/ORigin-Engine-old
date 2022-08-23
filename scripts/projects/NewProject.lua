os.chdir("Sandbox/sources/")
defaultArgument = 
[[#include <Origin.h>

namespace Origin {

  class Sandbox : public Application
  {
  public:

    // When Game Start
    void OnStart() override
    {
    }

    // When Game Looping
    void OnUpdate() override
    {
    }

  };

  Origin::Application* CreateApplication()
  {
    ORG_CORE_INFO("Application Created!");
    return new Sandbox();
  }
}]]

-- checking file available
if(os.isfile("Sandbox.cpp"))
then
    if(io.readfile("Sandbox.cpp") ~= defaultArgument)
    then
        print("\nOverwrite your project ? ") 
        print("Press [ Y / y ] key to Overwrite")
        print("Press [ Other ] key to Ignore")

        choice = io.read()
        if (choice == 'y')
            then
                print("\nCreating new project Sandbox.cpp file...")
                io.writefile("Sandbox.cpp", defaultArgument)
            end
        if (choice == 'Y')
            then
                print("\nCreating new project Sandbox.cpp file...")
                io.writefile("Sandbox.cpp", defaultArgument)
            end
    end
else
    io.writefile("Sandbox.cpp", defaultArgument)
end