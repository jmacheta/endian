include(CMakePackageConfigHelpers)

set(CMAKES_EXPORT_DIR cmake)

write_basic_package_version_file(endian-version.cmake COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/endian-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/endian-config.cmake
  INSTALL_DESTINATION ${CMAKES_EXPORT_DIR}
)

include(GNUInstallDirs)
install(TARGETS endian EXPORT endian_targets)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ecpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/endian-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/endian-version.cmake DESTINATION ${CMAKES_EXPORT_DIR}
)

install(EXPORT endian_targets NAMESPACE ecpp:: DESTINATION ${CMAKES_EXPORT_DIR})
