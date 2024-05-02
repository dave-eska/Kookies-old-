# CMake generated Testfile for 
# Source directory: /home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner
# Build directory: /home/dave/Documents/c++/Kookies/build/thirdparty/jsoncpp/src/jsontestrunner
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(jsoncpp_readerwriter "/usr/bin/python3.10" "-B" "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/../../test/runjsontests.py" "/home/dave/Documents/c++/Kookies/build/thirdparty/jsoncpp/src/jsontestrunner/jsontestrunner_exe" "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/../../test/data")
set_tests_properties(jsoncpp_readerwriter PROPERTIES  WORKING_DIRECTORY "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/../../test/data" _BACKTRACE_TRIPLES "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;43;add_test;/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;0;")
add_test(jsoncpp_readerwriter_json_checker "/usr/bin/python3.10" "-B" "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/../../test/runjsontests.py" "--with-json-checker" "/home/dave/Documents/c++/Kookies/build/thirdparty/jsoncpp/src/jsontestrunner/jsontestrunner_exe" "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/../../test/data")
set_tests_properties(jsoncpp_readerwriter_json_checker PROPERTIES  WORKING_DIRECTORY "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/../../test/data" _BACKTRACE_TRIPLES "/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;47;add_test;/home/dave/Documents/c++/Kookies/thirdparty/jsoncpp/src/jsontestrunner/CMakeLists.txt;0;")
