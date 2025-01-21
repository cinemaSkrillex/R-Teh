$directories = @(
    "GameEngine/include",
    "GameEngine/src",
    "RtypeServer/include",
    "RtypeServer/src",
    "RtypeGame/include",
    "RtypeGame/src",
    "Network/include",
    "Network/src"
)

foreach ($dir in $directories) {
    Get-ChildItem -Path $dir -Recurse -Include *.hpp,*.cpp | ForEach-Object {
        & clang-format -i $_.FullName
    }
}
