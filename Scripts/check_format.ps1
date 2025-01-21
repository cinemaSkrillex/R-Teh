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
