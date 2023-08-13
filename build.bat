@ECHO OFF

IF NOT [%1] == [] CALL SET build_type=%1
IF [%build_type%] == [] CALL SET build_type=Debug

IF NOT EXIST .\build mkdir .\build
IF NOT EXIST .\build\%build_type% mkdir .\build\%build_type%

cmake -B ./build/%build_type% -G "Ninja" -DCMAKE_BUILD_TYPE=%build_type%
cmake --build ./build/%build_type% --config %build_type%
