// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#define CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)  NAME##LINE
#define CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, LINE)    CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)
#define CUTE_DETAIL_UNIQUE_NAME(NAME)               CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, __LINE__)

#define CUTE_DETAIL_ASSERT(EXPR_EVAL, FILE, LINE, EXPR_TEXT, CAPS1, CAP2)                                                                               \
    try {                                                                                                                                               \
        ++cute::detail::eval_context::current().checks_performed;                                                                                       \
        if(!(EXPR_EVAL)) {                                                                                                                              \
            cute::detail::eval_context::current().add_exception(cute::detail::exception("", FILE, LINE, EXPR_TEXT, CAPS1, CAP2));                       \
        }                                                                                                                                               \
    } catch(cute::detail::exception const& ex) {                                                                                                        \
        cute::detail::eval_context::current().add_exception(ex);                                                                                        \
    } catch(std::exception const &ex) {                                                                                                                 \
        cute::detail::eval_context::current().add_exception(                                                                                            \
            cute::detail::exception("got an unexpected exception with message \"" + std::string(ex.what()) + "\"", FILE, LINE, EXPR_TEXT, CAPS1, CAP2)  \
        );                                                                                                                                              \
    } catch(...) {                                                                                                                                      \
        cute::detail::eval_context::current().add_exception(                                                                                            \
            cute::detail::exception("got an unexpected exception of unknown type", FILE, LINE, EXPR_TEXT, CAPS1, CAP2)                                  \
        );                                                                                                                                              \
    }

#define CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, EXCEPT)                                                                              \
    {                                                                                                                           \
        auto CUTE_DETAIL_UNIQUE_NAME(exception_ok) = false;                                                                     \
        try {                                                                                                                   \
            ++cute::detail::eval_context::current().checks_performed;                                                           \
            static_cast<void>(EXPR);                                                                                            \
        } catch(EXCEPT const&) {                                                                                                \
            CUTE_DETAIL_UNIQUE_NAME(exception_ok) = true;                                                                       \
        } catch(...) {                                                                                                          \
        }                                                                                                                       \
        if(!CUTE_DETAIL_UNIQUE_NAME(exception_ok)) {                                                                            \
            cute::detail::eval_context::current().add_exception(                                                                \
                cute::detail::exception("didn't get an expected exception of type \"" #EXCEPT "\"", __FILE__, __LINE__, #EXPR)  \
            );                                                                                                                  \
        }                                                                                                                       \
    }

#define CUTE_DETAIL_INIT()                                                                                          \
    cute::detail::test_registry& cute::detail::test_registry::instance() { static test_registry reg; return reg; }  \
    cute::detail::eval_context* cute::detail::eval_context::g_current = nullptr;


#define CUTE_DETAIL_TEST(...)                                                           \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)();                            \
    static bool CUTE_DETAIL_UNIQUE_NAME(cute_detail_test_reg) =                         \
        cute::detail::test_registry::instance().add(cute::test(                         \
            __FILE__, __LINE__, CUTE_DETAIL_UNIQUE_NAME(cute_detail_test), __VA_ARGS__  \
        )                                                                               \
    );                                                                                  \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)()

#define CUTE_DETAIL_DECOMPOSE(EXPR) static_cast<std::string>(cute::detail::decomposer()->* EXPR)
