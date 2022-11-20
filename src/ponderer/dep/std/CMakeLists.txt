add_library(ponderer_dep_std)
add_library(ponderer::dep::std ALIAS ponderer_dep_std)

target_sources(ponderer_dep_std INTERFACE)
