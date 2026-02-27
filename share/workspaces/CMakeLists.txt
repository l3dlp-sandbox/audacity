#
# Audacity: A Digital Audio Editor
#

# Workspaces
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${AU4_SHARE_NAME}${AU4_INSTALL_NAME}/workspaces
        FILES_MATCHING PATTERN "*.mws"
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".*" EXCLUDE
)
