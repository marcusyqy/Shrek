namespace shrek {

class Application
{
public:
    Application();

    // for linux based applications(?)
    Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv);
    ~Application();

    void Run();

private:
};

} // namespace shrek
