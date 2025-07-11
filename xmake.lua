add_rules("mode.debug", "mode.release")

add_requires("drogon", {
    configs = {
        cpp20 = true,
        openssl = true,
    }
})
add_requires("argparse")
add_requires("matchit")
add_requires("cppcodec")
add_requires("utfcpp")

target("bangumi-status-card")
    set_languages("cxx20")
    set_kind("binary")

    add_packages("drogon", {public = true})
    add_packages("argparse", {public = true})
    add_packages("matchit", {public = true})
    add_packages("cppcodec", {public = true})
    add_packages("utfcpp", {public = true})

    add_headerfiles("src/**.hpp")
    add_files("src/**.cpp")
    add_includedirs("src", {public = true})
    add_includedirs("public", {public = true})

    add_cxxflags("/utf-8", {public = true, tools = { "msvc", "cl" }})
    add_cxxflags("-finput-charset=UTF-8", {public = true, tools = { "clang", "gcc" }})    
    if is_plat("windows") then
        add_links("rpcrt4", {public = true})
    end
