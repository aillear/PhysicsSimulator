#pragma once

class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void Run();
    bool running;

  private:
    App();
    void Destory();
};


# define GET_App App::Instance()