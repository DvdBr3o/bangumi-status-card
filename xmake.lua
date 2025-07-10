add_rules("mode.debug", "mode.release")

add_requires("drogon", {
    configs = {
        cpp20 = true,
        openssl = true,
    }
})
add_requires("pugixml")
add_requires("argparse")

target("bangumi-status-card")
    set_languages("cxx20")
    set_kind("binary")

    add_packages("drogon", {public = true})
    add_packages("pugixml", {public = true})
    add_packages("argparse", {public = true})

    add_headerfiles("src/**.hpp")
    add_files("src/**.cpp")
    add_includedirs("src", {public = true})
    add_includedirs("public", {public = true})

    if is_plat("windows") then
        add_links("rpcrt4", {public = true})
    end
