include("D:/Qt Projects/Calculator/build/Desktop_Qt_6_12_0_MinGW_64_bit_Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Calculator-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "D:/Qt Projects/Calculator/build/Desktop_Qt_6_12_0_MinGW_64_bit_Debug/Calculator.exe"
    GENERATE_QT_CONF
)
