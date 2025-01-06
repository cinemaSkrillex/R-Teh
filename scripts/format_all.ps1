$directories = @(
    "game_engine/include",
    "game_engine/src",
    "rtype_server/include",
    "rtype_server/src",
    "rtype_game/include",
    "rtype_game/src",
    "network/include",
    "network/src"
)

foreach ($dir in $directories) {
    Get-ChildItem -Path $dir -Recurse -Include *.hpp,*.cpp | ForEach-Object {
        & clang-format -i $_.FullName
    }
}
