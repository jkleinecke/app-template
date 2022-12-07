

int main(int argc, char** argv)
{
    int result = 0;

    @autoreleasepool
    {
        os_main_init(argc, argv);

        result = app_main();
    }

    return(result);
}
