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

$hasErrors = $false

foreach ($dir in $directories) {
    Get-ChildItem -Path $dir -Recurse -Include *.hpp,*.cpp | ForEach-Object {
        Write-Host "Checking format for file: $($_.FullName)"
        $result = & clang-format --dry-run --Werror $_.FullName
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Formatting issue found in $($_.FullName)" -ForegroundColor Red
            $hasErrors = $true
        }
    }
}

if ($hasErrors) {
    Write-Host "Formatting issues detected!" -ForegroundColor Red
    exit 1
} else {
    Write-Host "All files are correctly formatted." -ForegroundColor Green
    exit 0
}
