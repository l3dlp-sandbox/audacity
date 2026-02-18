#
# Audacity: A Digital Audio Editor
#

add_subdirectory(locale)
add_subdirectory(autobotscripts)
add_subdirectory(icons)
add_subdirectory(workspaces)

IF (AU_MODULE_EFFECTS_NYQUIST)
    # Note: Nyquist runtime files (.lsp, .txt, .raw) are handled by au3/nyquist/CMakeLists.txt
#    add_subdirectory(nyquist) # should rename nyquist-runtime
    add_subdirectory(nyquist-plug-ins)
ENDIF()