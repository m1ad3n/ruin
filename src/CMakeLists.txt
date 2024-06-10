file(GLOB RUIN_SOURCES "*.c" "**/*.c")
file(GLOB RUIN_HEADERS "*.h" "**/*.h")
target_sources(ruin PRIVATE ${RUIN_SOURCES} ${RUIN_HEADERS})