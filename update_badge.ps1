$output = & "build\windows-release\UnlinkedTests.exe"
$match = $output | Select-String -Pattern "Test Results: (\d+)/\d+ test cases passed"
if ($match) {
    $passed = $match.Matches.Groups[1].Value
    $readme = Get-Content README.md -Raw
    $readme = $readme -replace "badge/tests-\d+%20passed-brightgreen", "badge/tests-$passed%20passed-brightgreen"
    [IO.File]::WriteAllText("README.md", $readme)
    Write-Host "Updated README badge to $passed passed."
} else {
    Write-Host "Could not parse test results."
}
