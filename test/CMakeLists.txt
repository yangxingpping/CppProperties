
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(CppProperties_test)
target_sources(CppProperties_test PRIVATE src/PropertyContainerHierarchy.cpp src/PropertyContainerTests.cpp src/PropertyTests.cpp src/ProxyPropertyTests.cpp)
target_link_libraries(CppProperties_test gtest gtest_main CppProperties::cppproperties)

# add our tests automatically to ctest which makes them discoverable by IDE's like MSVS
include(GoogleTest)
gtest_discover_tests(CppProperties_test)