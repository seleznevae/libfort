#include "tests.h"
#include "fort.h"


void test_error_codes(void)
{
    // Nonnegative code is success
    {
        assert_str_equal(ft_strerror(FT_SUCCESS), "Libfort success");
        assert_str_equal(ft_strerror(0), "Libfort success");
        assert_str_equal(ft_strerror(1), "Libfort success");
        assert_str_equal(ft_strerror(2), "Libfort success");
        assert_str_equal(ft_strerror(42), "Libfort success");
        assert_str_equal(ft_strerror(INT_MAX), "Libfort success");
    }

    // Error codes
    {
        assert_str_equal(ft_strerror(FT_MEMORY_ERROR), "Libfort error (out of memory)");
        assert_str_equal(ft_strerror(FT_EINVAL), "Libfort error (invalid argument)");
        assert_str_equal(ft_strerror(FT_INTERN_ERROR), "Libfort error (internal logic error)");
        assert_str_equal(ft_strerror(FT_GEN_ERROR), "Libfort error (general error)");

        assert_str_equal(ft_strerror(-42), "Libfort unknown error");
        assert_str_equal(ft_strerror(-666), "Libfort unknown error");
        assert_str_equal(ft_strerror(-INT_MAX), "Libfort unknown error");
    }
}
