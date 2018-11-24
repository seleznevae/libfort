#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP


namespace fort
{
class table;
}

int set_test_properties_as_default(void);
bool set_cpp_test_props_for_table(fort::table *table);
fort::table create_cpp_test_int_table(int set_test_opts);



#endif // TEST_UTILS_HPP
