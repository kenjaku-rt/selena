# Selena language example

use cli:io;

namespace main {
    type args = struct {
        num:usize,
        content:str[num]
    };

    type exit_status = enum {
        EXIT_SUCCESS,
        EXIT_FAILURE,
        @exit_status_ext
    };
};

main::exit_status@exit_status_ext = {
    EXIT_ERR1, EXIT_ERR2
};

use main::exit_status*;

MAIN(ARGS:(main::args)):i32 {

    var1:i64 = 10;

    cli:io.println("Selena app 1. You run it with:");

    for (i = 0; i < ARGS.num; i++) {
        cli:io.println("arg[{i}]: ", ARGS.content[i]);
    }
    
    cli:flush();

    if (var1->usize == ARGS.num) {
        return EXIT_ERR1;
    }

    return EXIT_SUCCESS;
}

