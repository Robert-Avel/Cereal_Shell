#ifndef PSTATUS_HPP
#define PSTATUS_HPP

namespace cs {
    enum PStatus {
        CONTINUE = -1, //           -1
        SUCCESS, //                  0
        FAIL, //                     1
        EXPECTED_NAME_FUNCTION, //   2
        LEXER_ERROR, //              3
        EXPECTED_SEMI_COLON, //      4
        KW_ARGS_ONLY_ERROR, //       5
        EXPECTED_EXPRESSION, //      6
        DASH_WITH_UNSIGNED, //        7
        NOT_FOUND,
        FUNCTION_NULL,
        NO_INPUT
    };
};

#endif
