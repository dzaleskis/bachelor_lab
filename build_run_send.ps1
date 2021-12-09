cmake cmake-build-release
cd cmake-build-release
make
./course_project_lab.exe
cd ..
node send_result "$PWD\cmake-build-release\result.json"