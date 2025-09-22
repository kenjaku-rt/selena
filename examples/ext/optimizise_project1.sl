# Optimizer package for Selena

use selena::extensor as ext;
use project1_optimizers as pr1;

# status / modified / old
ext::MAIN_IR(IR_CODE:(ext::ir)):(ext::exit_status; ext::ir; ext::ir) = {

    modified_ir:(ext::ir) = IR_CODE;

    switch(pr1::choose_opt(IR_CODE)) {
        case (pr1::opt_type::TYPE1) {
            # some actions
            ext::ir = pr1::optimize1(IR_CODE);
        }
        case (pr1::opt_type::TYPE2) {
            ext::ir = pr1::optimize2(IR_CODE);
        }
        case (pr1::opt_type::TYPE3) {
            ext::ir = pr1::optimize3(IR_CODE);
        }
    }

    return (main::exit::EXIT_SUCCESS, ext::ir, IR_CODE);
}