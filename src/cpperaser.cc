#include <generate_interface.h>
#include <parser.h>
#include <print_error.h>

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    Duck::Tokenizer t(argv[1]);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        print_error(interface.error());
        return -1;
    }

    generate_interface(interface.value());
}
